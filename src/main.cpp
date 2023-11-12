#include <charconv>
#include <sstream>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <fstream>

#include "sim.h"


using namespace llvm;


static inline constexpr const char * const INT_REGISTERS_GLOBAL_NAME = "INT_REGISTERS";
static inline constexpr const std::size_t INT_REGISTERS_NUMBER = 8;

static inline constexpr const char * const MEM_REGISTERS_GLOBAL_NAME = "MEM_REGISTERS";
static inline constexpr const std::size_t MEM_REGISTERS_NUMBER = 8;

static void to_lower(std::string & str) {
    std::transform(str.begin(), str.end(), str.begin(),[] (char c) {
        return std::tolower(c);
    });
}

template<typename T = int32_t, std::enable_if<std::is_integral_v<T>> * = nullptr>
static T get_imm(const std::string & value) {
    T result;

    if (std::from_chars(value.data(), value.data() + value.size(), result).ptr != value.data() + value.size()) {
        throw std::invalid_argument(value + " is not an immediate value");
    }

    return result;
}

static Function * generate_main(Module * const module, std::istream & is) {
    LLVMContext & context = module->getContext();

    IRBuilder<> builder(context);

    auto * const int_registers = module->getNamedGlobal(INT_REGISTERS_GLOBAL_NAME);
    auto * const mem_registers = module->getNamedGlobal(MEM_REGISTERS_GLOBAL_NAME);

    const auto sim_rand = module->getOrInsertFunction("sim_rand", builder.getInt32Ty());
    const auto sim_flush = module->getOrInsertFunction("sim_flush", builder.getVoidTy());
    const auto sim_set_pixel = module->getOrInsertFunction(
            "sim_set_pixel",
            builder.getVoidTy(),
            builder.getInt32Ty(),
            builder.getInt32Ty(),
            builder.getInt32Ty()
    );

    const auto dump_int_registers = module->getOrInsertFunction(
            "dump_int_registers",
            builder.getVoidTy(),
            builder.getPtrTy()
    );

    Function * const main_ = Function::Create(
            FunctionType::get(builder.getInt32Ty(), false),
            Function::ExternalLinkage,
            "main",
            module
    );

    builder.SetInsertPoint(BasicBlock::Create(context, "", main_));

    const auto load_int = [&builder] (auto * const reg) {
        auto * const load = builder.CreateLoad(builder.getInt32Ty(), reg);
        load->setAlignment(Align::Constant<4>());

        return load;
    };

    const auto load_ptr = [&builder] (auto * const reg) {
        return builder.CreateLoad(builder.getPtrTy(), reg);
    };

    const auto store_int = [&builder] (auto * const value, auto * const reg) {
        auto * const store = builder.CreateStore(value, reg);
        store->setAlignment(Align::Constant<4>());

        return store;
    };

    const auto store_ptr = [&builder] (auto * const value, auto * const reg) {
        return builder.CreateStore(value, reg);
    };

    std::unordered_map<std::string, std::pair<BasicBlock *, bool>> labels;

    const auto get_label = [&context, &main_, &labels] (const std::string & lbl) {
        if (auto it = labels.find(lbl); it != labels.end()) {
            return it->second.first;
        }

        auto * const new_block = BasicBlock::Create(context, lbl, main_);

        labels[lbl] = { new_block, false };
        return new_block;
    };

    const auto get_reg = [&builder] (
            const std::string & name,
            char prefix,
            auto * llvm_type,
            auto * llvm_ptr,
            std::size_t number,
            const char * kind
    ) {
        if (name[0] != prefix) {
            throw std::invalid_argument(name + " is not a " + kind + " register name");
        }

        std::size_t idx;
        if (std::from_chars(name.data() + 1, name.data() + name.size(), idx).ptr != name.data() + name.size()) {
            throw std::invalid_argument(name + " is not a " + kind + " register name");
        }

        if (idx > number) {
            throw std::invalid_argument(name + " is not a " + kind + " register name");
        }

        return builder.CreateInBoundsGEP(llvm_type, llvm_ptr, builder.getInt32(idx - 1));
    };

    const auto get_int_reg = [&builder, &int_registers, &get_reg] (const std::string & name) {
        return get_reg(name, 'i', builder.getInt32Ty(), int_registers, INT_REGISTERS_NUMBER, "int");
    };

    const auto get_mem_reg = [&builder, &mem_registers, &get_reg] (const std::string & name) {
        return get_reg(name, 'm', builder.getPtrTy(), mem_registers, MEM_REGISTERS_NUMBER, "memory");
    };

    const auto get_reg_or_imm = [&builder, &load_int, &get_int_reg] (const std::string & value) -> Value * {
        try {
            return builder.getInt32(get_imm(value));
        } catch (const std::invalid_argument &) {}

        try {
            return load_int(get_int_reg(value));
        } catch (const std::invalid_argument &) {}

        throw std::invalid_argument(value + " is not an immediate value or register name");
    };

    std::string line;
    std::size_t line_no = 0;
    bool was_branch = false;
    while (std::getline(is, line)) {
        ++line_no;

        std::istringstream line_is { line };

        std::string opcode;
        std::vector<std::string> args(1);

        if (!(line_is >> opcode)) {
            continue;
        }

        if (opcode[0] == '#') {
            continue;
        }

        while (line_is >> *args.rbegin()) {
            args.emplace_back("");
        }

        args.pop_back();

        const auto check_args = [&opcode, &args, line_no] (std::size_t size) {
            if (args.size() < size) {
                throw std::invalid_argument("not enough arguments for instruction " + opcode
                    + " on line " + std::to_string(line_no));
            }

            if (args.size() > size) {
                throw std::invalid_argument("too many arguments for instruction " + opcode
                    + " on line " + std::to_string(line_no));
            }
        };

        outs() << "INSTRUCTION " << opcode << " OPERANDS ";
        for (auto & s : args) {
            outs() << s << ' ';
        }
        outs() << '\n';

        if (*opcode.rbegin() == ':') {
            check_args(0);

            opcode.resize(opcode.size() - 1);

            BasicBlock * next_block;

            if (auto it = labels.find(opcode); it != labels.end()) {
                if (it->second.second) {
                    throw std::invalid_argument("redefinition of label " + opcode + " on line "
                        + std::to_string(line_no));
                }

                it->second.second = true;
                next_block = it->second.first;
            } else {
                next_block = BasicBlock::Create(context, opcode, main_);

                labels[opcode] = { next_block, true };
            }

            if (!was_branch) {
                builder.CreateBr(next_block);
            }

            builder.SetInsertPoint(next_block);
            was_branch = false;
            continue;
        }

        to_lower(opcode);

        // control flow

        if (opcode == "goto") {
            check_args(1);

            builder.CreateBr(get_label(args[0]));
            was_branch = true;
            continue;
        }

        if (opcode == "if") {
            check_args(2);

            auto * const value = get_reg_or_imm(args[0]);
            auto * const tbr = get_label(args[1]);

            auto * const cond = builder.CreateICmp(llvm::CmpInst::ICMP_NE, value, builder.getInt32(0));
            auto * const fbr = BasicBlock::Create(context, "", main_);
            builder.CreateCondBr(cond, tbr, fbr);

            builder.SetInsertPoint(fbr);
            continue;
        }

        // int registers

        if (opcode == "movi") {
            check_args(2);
            store_int(get_reg_or_imm(args[0]), get_int_reg(args[1]));
            continue;
        }

        if (opcode == "addi") {
            check_args(2);

            auto * const add = get_reg_or_imm(args[0]);
            auto * const reg = get_int_reg(args[1]);

            store_int(builder.CreateAdd(add, load_int(reg)), reg);
            continue;
        }

        if (opcode == "mul") {
            check_args(2);

            auto * const mul = get_reg_or_imm(args[0]);
            auto * const reg = get_int_reg(args[1]);

            store_int(builder.CreateMul(mul, load_int(reg)), reg);
            continue;
        }

        if (opcode == "and") {
            check_args(2);

            auto * const mask = get_reg_or_imm(args[0]);
            auto * const reg = get_int_reg(args[1]);

            store_int(builder.CreateAnd(mask, load_int(reg)), reg);
            continue;
        }

        if (opcode == "neq") {
            check_args(3);

            auto * const x = get_reg_or_imm(args[0]);
            auto * const y = get_reg_or_imm(args[1]);
            auto * const reg = get_int_reg(args[2]);

            auto * const res = builder.CreateICmp(llvm::CmpInst::ICMP_NE, x, y);
            store_int(builder.CreateZExt(res, builder.getInt32Ty()), reg);
            continue;
        }

        // memory

        if (opcode == "buffer") {
            check_args(2);

            const auto size = get_imm<std::size_t>(args[0]);
            const auto arr_type = ArrayType::get(builder.getInt32Ty(), size);

            store_ptr(builder.CreateAlloca(arr_type), get_mem_reg(args[1]));
            continue;
        }

        if (opcode == "store") {
            check_args(2);
            store_int(get_reg_or_imm(args[0]), load_ptr(get_mem_reg(args[1])));
            continue;
        }

        if (opcode == "movm") {
            check_args(2);
            store_ptr(load_ptr(get_mem_reg(args[0])), get_mem_reg(args[1]));
            continue;
        }

        if (opcode == "addm") {
            check_args(2);

            auto * const add = get_reg_or_imm(args[0]);
            auto * const reg = get_mem_reg(args[1]);

            auto * const size_add = builder.CreateMul(add, builder.getInt32(4));
            auto * const old = load_ptr(reg);

            store_ptr(builder.CreateAdd(size_add, old), reg);
            continue;
        }

        // bitset

        if (opcode == "get_bit") {
            check_args(3);

            auto * const ptr = load_ptr(get_mem_reg(args[0]));
            auto * const idx = get_reg_or_imm(args[1]);
            auto * const reg = get_int_reg(args[2]);

            auto * const high_idx = builder.CreateLShr(idx, builder.getInt32(5));
            auto * const low_idx = builder.CreateAnd(idx, builder.getInt32(31));

            auto * const cell_ptr = builder.CreateInBoundsGEP(builder.getInt32Ty(), ptr, high_idx);
            auto * const cell_value = load_int(cell_ptr);

            auto * const shr_value = builder.CreateLShr(cell_value, low_idx);
            auto * const value = builder.CreateAnd(shr_value, builder.getInt32(1));

            store_int(value, reg);
            continue;
        }

        if (opcode == "set_bit") {
            check_args(2);

            auto * const ptr = load_ptr(get_mem_reg(args[0]));
            auto * const idx = get_reg_or_imm(args[1]);

            auto * const high_idx = builder.CreateLShr(idx, builder.getInt32(5));
            auto * const low_idx = builder.CreateAnd(idx, builder.getInt32(31));

            auto * const cell_ptr = builder.CreateInBoundsGEP(builder.getInt32Ty(), ptr, high_idx);
            auto * const cell_value = load_int(cell_ptr);

            auto * const shl_1 = builder.CreateShl(builder.getInt32(1), low_idx);
            auto * const value = builder.CreateOr(cell_value, shl_1);

            store_int(value, cell_ptr);
            continue;
        }

        if (opcode == "clear_bit") {
            check_args(2);

            auto * const ptr = load_ptr(get_mem_reg(args[0]));
            auto * const idx = get_reg_or_imm(args[1]);

            auto * const high_idx = builder.CreateLShr(idx, builder.getInt32(5));
            auto * const low_idx = builder.CreateAnd(idx, builder.getInt32(31));

            auto * const cell_ptr = builder.CreateInBoundsGEP(builder.getInt32Ty(), ptr, high_idx);
            auto * const cell_value = load_int(cell_ptr);

            auto * const shl_1 = builder.CreateShl(builder.getInt32(1), low_idx);
            auto * const neg_shl_1 = builder.CreateXor(shl_1, builder.getInt32(-1));
            auto * const value = builder.CreateAnd(cell_value, neg_shl_1);

            store_int(value, cell_ptr);
            continue;
        }

        // misc

        if (opcode == "rand") {
            check_args(1);
            store_int(builder.CreateCall(sim_rand), get_int_reg(args[0]));
            continue;
        }

        if (opcode == "set_pixel") {
            check_args(3);

            auto * const x = get_reg_or_imm(args[0]);
            auto * const y = get_reg_or_imm(args[1]);
            auto * const v = get_reg_or_imm(args[2]);

            auto * const bool_v = builder.CreateICmp(llvm::CmpInst::ICMP_EQ, v, builder.getInt32(1));
            auto * const int_v = builder.CreateZExt(bool_v, builder.getInt32Ty());
            auto * const color_v = builder.CreateMul(int_v, builder.getInt32(0xFF00));
            auto * const color = builder.CreateAdd(color_v, builder.getInt32(0xFF000000));

            builder.CreateCall(sim_set_pixel, { x, y, color });
            continue;
        }

        if (opcode == "flush") {
            check_args(0);
            builder.CreateCall(sim_flush);
            continue;
        }

        if (opcode == "dumpi") {
            check_args(0);
            builder.CreateCall(dump_int_registers, { int_registers });
            continue;
        }

        throw std::invalid_argument("unknown opcode " + opcode);
    }

    builder.CreateRet(builder.getInt32(0));

    for (auto & [lbl, b] : labels) {
        if (!b.second) {
            throw std::invalid_argument("label " + lbl + " is not defined");
        }
    }

    return main_;
}

[[noreturn]]
static void stack_chk_fail_mock() {
    throw std::runtime_error("stack check failed");
}

static void dump_int_registers(int32_t * int_registers) {
    for (std::size_t i = 0; i < INT_REGISTERS_NUMBER; ++i) {
        outs() << int_registers[i] << ' ';
    }

    outs() << '\n';
}

int main(int argc, char * argv[]) {
    if (argc < 2) {
        errs() << "usage: " << argv[0] << " <input file>\n";
        return 1;
    }

    std::ifstream input_file { argv[1] };
    if (!input_file.is_open()) {
        errs() << "cannot open file " << argv[1] << '\n';
        return 2;
    }

    LLVMContext context;

    auto * const module = new Module("src/main.c", context);

    module->getOrInsertGlobal(
            INT_REGISTERS_GLOBAL_NAME,
            ArrayType::get(IntegerType::getInt32Ty(context), INT_REGISTERS_NUMBER)
    );

    module->getOrInsertGlobal(
            MEM_REGISTERS_GLOBAL_NAME,
            ArrayType::get(IntegerType::getInt32Ty(context), MEM_REGISTERS_NUMBER)
    );

    auto * const main_function = generate_main(module, input_file);

    std::error_code err;
    raw_fd_stream fos { "main.gen.ll", err };
    module->print(fos, nullptr);

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

    ExecutionEngine * const ee = EngineBuilder(std::unique_ptr<Module>(module)).create();

    ee->InstallLazyFunctionCreator([&](const std::string & name) -> void * {
        if (name == "sim_set_pixel") {
            return reinterpret_cast<void *>(sim_set_pixel);
        }

        if (name == "sim_flush") {
            return reinterpret_cast<void *>(sim_flush);
        }

        if (name == "sim_rand") {
            return reinterpret_cast<void *>(sim_rand);
        }

        if (name == "dump_int_registers") {
            return reinterpret_cast<void *>(dump_int_registers);
        }

        if (name == "__stack_chk_fail") {
            return reinterpret_cast<void *>(stack_chk_fail_mock);
        }

        outs() << "Not found " << name << "\n";
        return nullptr;
    });

    int32_t int_registers_buffer[INT_REGISTERS_NUMBER];
    void * mem_registers_buffer[INT_REGISTERS_NUMBER];

    auto * const int_registers = module->getNamedGlobal(INT_REGISTERS_GLOBAL_NAME);
    ee->addGlobalMapping(int_registers, int_registers_buffer);

    auto * const mem_registers = module->getNamedGlobal(MEM_REGISTERS_GLOBAL_NAME);
    ee->addGlobalMapping(mem_registers, mem_registers_buffer);

    ee->finalizeObject();

    const GenericValue result = ee->runFunction(main_function, {});

    return static_cast<int>(*result.IntVal.getRawData());
}

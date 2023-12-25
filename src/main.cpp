#include <unordered_map>
#include <sstream>
#include <fstream>
#include <memory>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

#include "sim.h"
#include "ast.hpp"

extern "C" {
#include "parser.h"
#include "lexer.h"
}


using namespace llvm;

struct compiler_context {

    Module * llvm_module;
    IRBuilder<> ir_builder;
    std::unordered_map<std::string, FunctionType *> llvm_function_types;
    std::unordered_map<std::shared_ptr<ast_expr>, BasicBlock *> llvm_basic_blocks;
    std::vector<std::pair<BasicBlock *, BasicBlock *>> loops;
    Function * current_function = nullptr;

    BasicBlock * create_basic_block() const {
        return BasicBlock::Create(llvm_module->getContext(), "", current_function);
    }

    BasicBlock * get_basic_block(std::shared_ptr<ast_expr> expr) {
        if (auto it = llvm_basic_blocks.find(expr); it != llvm_basic_blocks.end()) {
            return it->second;
        }

        auto * const result = create_basic_block();
        llvm_basic_blocks.emplace(std::move(expr), result);

        return result;
    }

    Value * get_trivial_value() {
        return ir_builder.getInt32(0);
    }
};

struct variable {

    std::shared_ptr<ast_type> type;
    Value * llvm_value = nullptr;
};

struct scope {

    const scope * parent;
    std::unordered_map<std::string, variable> variables;

    const variable & lookup(const std::string & id) const {
        const struct scope * scope = this;

        while (scope) {
            if (auto it = scope->variables.find(id); it != scope->variables.end()) {
                return it->second;
            }

            scope = scope->parent;
        }

        throw std::invalid_argument { "no variable named " + id + " in current scope" };
    }
};


[[noreturn]]
static void stack_chk_fail_mock() {
    throw std::runtime_error("stack check failed");
}

static std::shared_ptr<ast_program> parse_program(char * filename) {
    FILE * const input_file = fopen(filename, "r");

    if (!input_file) {
        std::string error { strerror(errno) };
        throw std::runtime_error { "Bad input file" + std::move(error) };
    }

    try {
        const YY_BUFFER_STATE buffer_state = yy_create_buffer(input_file, 4096); // NOLINT(*-misplaced-const)
        yy_switch_to_buffer(buffer_state);

        ast_program * ast = nullptr;
        char * error = nullptr;

        switch (yyparse(&ast, &error)) {
            case 1:
                throw std::runtime_error { "Parsing failed: " + std::string {error} };

            case 2:
                throw std::runtime_error { "Memory exhausted" };
        }

        return std::shared_ptr<ast_program> { ast };
    } catch (...) {
        fclose(input_file);
        throw;
    }
}

static void resolve_types(const std::shared_ptr<ast_decl> & decl) {
    //
}

static void resolve_types(const std::shared_ptr<ast_program> & program) {
    for (const auto & decl : program->decls) {
        resolve_types(decl);
    }
}

static Type * compile(compiler_context & ctx, const std::shared_ptr<ast_type> & type) {
    if (std::dynamic_pointer_cast<ast_type_int>(type)) {
        return ctx.ir_builder.getInt32Ty();
    }

    if (std::dynamic_pointer_cast<ast_type_array>(type)) {
        return ctx.ir_builder.getPtrTy();
    }

    throw std::invalid_argument { "unsupported type" };
}

static Value * compile_lvalue(
    compiler_context & ctx,
    const std::shared_ptr<ast_lvalue_expr> & expr,
    scope & parent_scope,
    BasicBlock * next_basic_block
);

static Value * compile_lvalue0(
    compiler_context & ctx,
    const std::shared_ptr<ast_lvalue_expr> & expr,
    scope & scope,
    BasicBlock * next_basic_block
);

static Value * compile(
    compiler_context & ctx,
    std::shared_ptr<ast_expr> expr,
    scope & parent_scope,
    BasicBlock * next_basic_block
) {
    scope scope { &parent_scope };

    while (true) {
        if (!expr) {
            throw std::invalid_argument { "null expr" };
        }

        auto * const basic_block = ctx.get_basic_block(expr);
        ctx.ir_builder.SetInsertPoint(basic_block);

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_seq>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->lhs));

            compile(ctx, ptr->lhs, scope, ctx.get_basic_block(ptr->rhs));
            expr = ptr->rhs;
            continue;
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_var_array>(expr)) {
            auto * const var = ctx.ir_builder.CreateAlloca(compile(ctx, ptr->element_type), ptr->size);
            ctx.ir_builder.CreateBr(next_basic_block);

            parent_scope.variables[ptr->id] = { ptr->var_type, var };
            return ctx.get_trivial_value();
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_var>(expr)) {
            auto * const var = ctx.ir_builder.CreateAlloca(compile(ctx, ptr->var_type));
            ctx.ir_builder.CreateBr(next_basic_block);

            parent_scope.variables[ptr->id] = { ptr->var_type, var };
            return ctx.get_trivial_value();
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_if>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->cond));

            auto * const br_basic_block = ctx.create_basic_block();
            auto * const result_basic_block = ctx.create_basic_block();

            auto * const cond = compile(ctx, ptr->cond, scope, br_basic_block);

            ctx.ir_builder.SetInsertPoint(br_basic_block);
            ctx.ir_builder.CreateCondBr(
                cond,
                ctx.get_basic_block(ptr->then_branch),
                ctx.get_basic_block(ptr->else_branch)
            );

            auto * const then_result = compile(ctx, ptr->then_branch, scope, result_basic_block);
            auto * const else_result = compile(ctx, ptr->else_branch, scope, result_basic_block);

            ctx.ir_builder.SetInsertPoint(result_basic_block);
            auto * const result = ctx.ir_builder.CreatePHI(compile(ctx, ptr->type), 2);
            result->addIncoming(then_result, ctx.get_basic_block(ptr->then_branch));
            result->addIncoming(else_result, ctx.get_basic_block(ptr->else_branch));
            ctx.ir_builder.CreateBr(next_basic_block);
            return result;
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_while>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->cond));

            auto * const br_basic_block = ctx.create_basic_block();

            auto * const cond = compile(ctx, ptr->cond, scope, br_basic_block);

            ctx.ir_builder.SetInsertPoint(br_basic_block);
            ctx.ir_builder.CreateCondBr(cond, ctx.get_basic_block(ptr->body), next_basic_block);

            ctx.loops.emplace_back(ctx.get_basic_block(ptr->cond), next_basic_block);
            compile(ctx, ptr->body, scope, ctx.get_basic_block(ptr->cond));
            ctx.loops.pop_back();

            return ctx.get_trivial_value();
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_do>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->body));

            ctx.loops.emplace_back(ctx.get_basic_block(ptr->cond), next_basic_block);
            compile(ctx, ptr->body, scope, ctx.get_basic_block(ptr->cond));
            ctx.loops.pop_back();

            auto * const br_basic_block = ctx.create_basic_block();
            auto * const cond = compile(ctx, ptr->cond, scope, br_basic_block);

            ctx.ir_builder.SetInsertPoint(br_basic_block);
            ctx.ir_builder.CreateCondBr(cond, ctx.get_basic_block(ptr->body), next_basic_block);
            return ctx.get_trivial_value();
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_for>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->cond));

            auto * const br_basic_block = ctx.create_basic_block();

            auto * const cond = compile(ctx, ptr->cond, scope, br_basic_block);

            ctx.ir_builder.SetInsertPoint(br_basic_block);
            ctx.ir_builder.CreateCondBr(cond, ctx.get_basic_block(ptr->body), next_basic_block);

            ctx.loops.emplace_back(ctx.get_basic_block(ptr->post), next_basic_block);
            compile(ctx, ptr->body, scope, ctx.get_basic_block(ptr->post));
            ctx.loops.pop_back();

            compile(ctx, ptr->post, scope, ctx.get_basic_block(ptr->cond));
            return ctx.get_trivial_value();
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_lvalue_expr>(expr)) {
            auto * const load_basic_block = ctx.create_basic_block();

            auto * const result_ptr = compile_lvalue0(ctx, ptr, scope, load_basic_block);

            ctx.ir_builder.SetInsertPoint(load_basic_block);
            auto * const result = ctx.ir_builder.CreateLoad(compile(ctx, ptr->type), result_ptr);
            ctx.ir_builder.CreateBr(next_basic_block);
            return result;
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_literal>(expr)) {
            ctx.ir_builder.CreateBr(next_basic_block);

            return ctx.ir_builder.getInt32(ptr->value);
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_assign>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->target));

            auto * const target = compile_lvalue(ctx, ptr->target, scope, ctx.get_basic_block(ptr->value));

            auto * const store_basic_block = ctx.create_basic_block();

            auto * const value = compile(ctx, ptr->value, scope, store_basic_block);

            ctx.ir_builder.SetInsertPoint(store_basic_block);
            ctx.ir_builder.CreateStore(value, target);
            ctx.ir_builder.CreateBr(next_basic_block);
            return value;
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_binary>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->lhs));

            auto * const lhs = compile(ctx, ptr->lhs, scope, ctx.get_basic_block(ptr->rhs));

            auto * const binop_basic_block = ctx.create_basic_block();

            auto * const rhs = compile(ctx, ptr->rhs, scope, binop_basic_block);

            ctx.ir_builder.SetInsertPoint(binop_basic_block);
            Value * result;

            switch (ptr->op) {
                case AST_EXPR_BINOP_ADD:
                    result = ctx.ir_builder.CreateAdd(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_SUB:
                    result = ctx.ir_builder.CreateSub(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_MUL:
                    result = ctx.ir_builder.CreateMul(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_DIV:
                    result = ctx.ir_builder.CreateSDiv(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_REM:
                    result = ctx.ir_builder.CreateSRem(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_BAND:
                    result = ctx.ir_builder.CreateAnd(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_BXOR:
                    result = ctx.ir_builder.CreateXor(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_BOR:
                    result = ctx.ir_builder.CreateOr(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_SHL:
                    result = ctx.ir_builder.CreateShl(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_SHR:
                    result = ctx.ir_builder.CreateAShr(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_AND: {
                    auto * const lhs_bool = ctx.ir_builder.CreateIsNotNull(lhs);
                    auto * const rhs_bool = ctx.ir_builder.CreateIsNotNull(rhs);

                    auto * const result_bool = ctx.ir_builder.CreateAnd(lhs_bool, rhs_bool);
                    result = ctx.ir_builder.CreateZExt(result_bool, ctx.ir_builder.getInt32Ty());
                    break;
                }

                case AST_EXPR_BINOP_OR: {
                    auto * const result0 = ctx.ir_builder.CreateOr(lhs, rhs);
                    auto * const result_bool = ctx.ir_builder.CreateIsNotNull(result0);
                    result = ctx.ir_builder.CreateZExt(result_bool, ctx.ir_builder.getInt32Ty());
                    break;
                }

                case AST_EXPR_BINOP_EQ:
                    result = ctx.ir_builder.CreateICmpEQ(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_NE:
                    result = ctx.ir_builder.CreateICmpNE(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_LT:
                    result = ctx.ir_builder.CreateICmpSLT(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_LE:
                    result = ctx.ir_builder.CreateICmpSLE(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_GT:
                    result = ctx.ir_builder.CreateICmpSGT(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_GE:
                    result = ctx.ir_builder.CreateICmpSGE(lhs, rhs);
                    break;
            }

            ctx.ir_builder.CreateBr(next_basic_block);
            return result;
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_unary>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->value));

            auto * const unop_basic_block = ctx.create_basic_block();

            auto * const value = compile(ctx, ptr->value, scope, unop_basic_block);

            ctx.ir_builder.SetInsertPoint(unop_basic_block);
            Value * result;

            switch (ptr->op) {
                case AST_EXPR_UNOP_NEG:
                    result = ctx.ir_builder.CreateNeg(value);
                    break;

                case AST_EXPR_UNOP_BINV:
                    result = ctx.ir_builder.CreateXor(value, Constant::getNullValue(value->getType()));
                    break;

                case AST_EXPR_UNOP_INV: {
                    auto * const result_bool = ctx.ir_builder.CreateIsNull(value);
                    result = ctx.ir_builder.CreateZExt(result_bool, ctx.ir_builder.getInt32Ty());
                    break;
                }
            }

            ctx.ir_builder.CreateBr(next_basic_block);
            return result;
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_call>(expr)) {
            std::vector<Value *> args(ptr->args.size());

            if (!ptr->args.empty()) {
                ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->args[0]));

                for (std::size_t i = 0; i < args.size() - 1; ++i) {
                    args[i] = compile(ctx, ptr->args[i], scope, ctx.get_basic_block(ptr->args[i + 1]));
                }

                auto * const call_basic_block = ctx.create_basic_block();

                args[args.size() - 1] = compile(ctx, ptr->args[args.size() - 1], scope, call_basic_block);

                ctx.ir_builder.SetInsertPoint(call_basic_block);
            }

            auto * const result = ctx.ir_builder.CreateCall(
                ctx.llvm_module->getOrInsertFunction(ptr->id, ctx.llvm_function_types[ptr->id]),
                args
            );

            ctx.ir_builder.CreateBr(next_basic_block);
            return result;
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_return>(expr)) {
            if (ptr->value) {
                ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->value));

                auto * const ret_basic_block = ctx.create_basic_block();

                auto * const result = compile(ctx, ptr->value, scope, ret_basic_block);

                ctx.ir_builder.SetInsertPoint(ret_basic_block);
                ctx.ir_builder.CreateRet(result);
            } else {
                ctx.ir_builder.CreateRet(Constant::getNullValue(ctx.current_function->getReturnType()));
            }

            // dummy value
            return ctx.ir_builder.getInt32(0);
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_break>(expr)) {
            if (ctx.loops.empty()) {
                throw std::invalid_argument { "break is only allowed in loops" };
            }

            ctx.ir_builder.CreateBr(ctx.loops.rbegin()->second);

            // dummy value
            return ctx.ir_builder.getInt32(0);
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_continue>(expr)) {
            if (ctx.loops.empty()) {
                throw std::invalid_argument { "continue is only allowed in loops" };
            }

            ctx.ir_builder.CreateBr(ctx.loops.rbegin()->first);

            // dummy value
            return ctx.ir_builder.getInt32(0);
        }

        throw std::invalid_argument { "unsupported expr" };
    }
}

static Value * compile_lvalue(
    compiler_context & ctx,
    const std::shared_ptr<ast_lvalue_expr> & expr,
    scope & parent_scope,
    BasicBlock * next_basic_block
) {
    scope scope { &parent_scope };

    return compile_lvalue0(ctx, expr, scope, next_basic_block);
}

static Value * compile_lvalue0(
    compiler_context & ctx,
    const std::shared_ptr<ast_lvalue_expr> & expr,
    scope & scope,
    BasicBlock * next_basic_block
) {
    auto * const basic_block = ctx.get_basic_block(expr);
    ctx.ir_builder.SetInsertPoint(basic_block);

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_name>(expr)) {
        ctx.ir_builder.CreateBr(next_basic_block);

        return scope.lookup(ptr->id).llvm_value;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_subscript>(expr)) {
        ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->value));

        auto * const value = compile(ctx, ptr->value, scope, ctx.get_basic_block(ptr->index));

        auto * const gep_basic_block = ctx.create_basic_block();

        auto * const index = compile(ctx, ptr->index, scope, gep_basic_block);

        ctx.ir_builder.SetInsertPoint(gep_basic_block);
        auto * const result = ctx.ir_builder.CreateInBoundsGEP(compile(ctx, ptr->type), value, index);
        ctx.ir_builder.CreateBr(next_basic_block);

        return result;
    }

    throw std::invalid_argument { "unsupported lvalue expr" };
}

static void compile(compiler_context & ctx, const std::shared_ptr<ast_decl> & decl) {
    if (!decl->body) {
        return;
    }

    auto * const function = Function::Create(
        ctx.llvm_function_types[decl->id],
        Function::ExternalLinkage,
        decl->id,
        ctx.llvm_module
    );

    ctx.current_function = function;

    auto * const first_basic_block = ctx.create_basic_block();
    ctx.ir_builder.SetInsertPoint(first_basic_block);

    scope initial_scope { nullptr };

    {
        auto it = function->args().begin();
        for (const auto &arg: decl->args) {
            auto * const llvm_arg = it++;

            llvm_arg->setName(arg.id);

            auto * var = ctx.ir_builder.CreateAlloca(compile(ctx, arg.type));
            ctx.ir_builder.CreateStore(llvm_arg, var);

            initial_scope.variables[arg.id] = { arg.type, var };
        }
    }

    ctx.ir_builder.CreateBr(ctx.get_basic_block(decl->body));

    auto * const last_basic_block = ctx.create_basic_block();

    auto * const result = compile(ctx, decl->body, initial_scope, last_basic_block);

    ctx.ir_builder.SetInsertPoint(last_basic_block);
    ctx.ir_builder.CreateRet(result);
}

static void compile(const std::shared_ptr<ast_program> & program, Module * module) {
    compiler_context ctx { module, IRBuilder { module->getContext() } };

    for (const auto & decl: program->decls) {
        std::vector<Type *> params;

        for (const auto & arg : decl->args) {
            params.emplace_back(compile(ctx, arg.type));
        }

        ctx.llvm_function_types[decl->id] = FunctionType::get(
            compile(ctx, decl->return_type),
            params,
            false
        );
    }

    for (const auto & item: program->decls) {
        compile(ctx, item);
    }
}

int main(int argc, char * argv[]) {
    if (argc < 2) {
        errs() << "usage: " << argv[0] << " <input file>\n";
        return 1;
    }

    std::shared_ptr<ast_program> program;

    try {
        program = parse_program(argv[1]);
    } catch (const std::exception & e) {
        errs() << e.what() << '\n';
        return 2;
    }

    LLVMContext context;
    auto * const module = new Module("main", context);

    resolve_types(program);
    compile(program, module);

    std::error_code err;
    raw_fd_stream fos { "main.gen.ll", err };
    module->print(fos, nullptr);

    return 0;

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

        if (name == "__stack_chk_fail") {
            return reinterpret_cast<void *>(stack_chk_fail_mock);
        }

        errs() << "Not found " << name << "\n";
        return nullptr;
    });

    ee->finalizeObject();

    // const GenericValue result = ee->runFunction(main_function, {});

    // return static_cast<int>(*result.IntVal.getRawData());
}

#include <charconv>
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


[[noreturn]]
static void stack_chk_fail_mock() {
    throw std::runtime_error("stack check failed");
}

std::shared_ptr<ast_program> parse_program(char * filename) {
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

    return 0;

    LLVMContext context;

    auto * const module = new Module("main", context);

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

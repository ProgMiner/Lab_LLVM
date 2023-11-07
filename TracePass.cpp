#include <llvm/Support/raw_ostream.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/IR/Function.h>


using namespace llvm;


namespace {

struct TracePass : public PassInfoMixin<TracePass> {

    static inline constexpr const char * const LOG_FUNCTION_NAME = "log_llvm_ir";

    PreservedAnalyses run(Function & function, FunctionAnalysisManager &) {
        IRBuilder<> builder(function.getContext());

        Module * const module = function.getParent();
        const FunctionCallee logFunction = module->getOrInsertFunction(
                LOG_FUNCTION_NAME,
                builder.getVoidTy(),
                builder.getInt8PtrTy()
        );

        for (BasicBlock & block : function) {
            for (Instruction & instruction : block) {
                if (dyn_cast<PHINode>(&instruction)) {
                    continue;
                }

                std::string text;
                raw_string_ostream os { text };
                instruction.print(os, false);

                builder.SetInsertPoint(&instruction);
                builder.CreateCall(logFunction, { builder.CreateGlobalStringPtr(text) });
            }
        }

        return PreservedAnalyses::all();
    }
};

} // anonymous namespace

static void registerTracePass(FunctionPassManager &FPM, OptimizationLevel O) {
    FPM.addPass(TracePass());
}

extern "C" PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION,
        "Trace Pass",
        "v0.1",
        [](PassBuilder &PB) {
            PB.registerVectorizerStartEPCallback(registerTracePass);
        }
    };
}

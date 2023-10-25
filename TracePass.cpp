#include <llvm/Support/raw_ostream.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/IR/Function.h>


using namespace llvm;


namespace {

struct TracePass : public PassInfoMixin<TracePass> {

    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
        outs() << "In a function called " << F.getName() << "!\n";

        outs() << "Function body:\n";
        F.print(llvm::outs());

        for (auto &B : F) {
            outs() << "Basic block:\n";
            B.print(llvm::outs());

            for (auto &I : B) {
                outs() << "Instruction: \n";
                I.print(llvm::outs(), true);
                outs() << "\n";
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

#include <llvm/IR/Function.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace {

struct SkeletonPass : public PassInfoMixin<SkeletonPass> {

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

static void registerSkeletonPass(FunctionPassManager &FPM, OptimizationLevel O) {
    FPM.addPass(SkeletonPass());
}

extern "C" PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION,
        "Skeleton Pass",
        "v0.1",
        [](PassBuilder &PB) {
            PB.registerVectorizerStartEPCallback(registerSkeletonPass);
        }
    };
}

#include "pointer-deref.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"

using namespace llvm;

PreservedAnalyses PointerDerefPass::run(Function &F, FunctionAnalysisManager &FAM)
{
    errs() << "Function: " << F.getName() << "\n";
    errs() << "The function takes " << F.arg_size() << " arguments" << "\n";
    return PreservedAnalyses::all();
}

// -----

bool shouldRegister(StringRef name, FunctionPassManager &FPM,
    ArrayRef<PassBuilder::PipelineElement> _)
{
    if (name == "pointer-deref") {
        FPM.addPass(PointerDerefPass());
        return true;
    }

    return false;
}

void registerPassBuilderCallbacks(PassBuilder &PB)
{
    PB.registerPipelineParsingCallback(shouldRegister);
}

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo()
{
    PassPluginLibraryInfo pluginInfo;

    pluginInfo.APIVersion = LLVM_PLUGIN_API_VERSION;
    pluginInfo.PluginName = "Pointer Dereference Observer";
    pluginInfo.PluginVersion = LLVM_VERSION_STRING;
    pluginInfo.RegisterPassBuilderCallbacks = registerPassBuilderCallbacks;

    return pluginInfo;
}


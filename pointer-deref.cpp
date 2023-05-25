#include "pointer-deref.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include <cstdint>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>

using namespace llvm;

PreservedAnalyses PointerDerefPass::run(Function &F, FunctionAnalysisManager &FAM)
{
    if (F.getName() == "pointer_deref")
        return PreservedAnalyses::all();

    errs() << "Function: " << F.getName() << "\n";
    errs() << "The function takes " << F.arg_size() << " arguments" << "\n";

    LLVMContext &Ctx = F.getContext();
    IRBuilder<> Builder(Ctx);

    FunctionType *HookFuncType = FunctionType::get(Type::getVoidTy(Ctx),
        {Type::getInt8PtrTy(Ctx), Type::getInt8Ty(Ctx)}, false);
    FunctionCallee HookFunc =
        F.getParent()->getOrInsertFunction("pointer_deref", HookFuncType);

    for (auto &BB: F) {
        for (auto &I: BB) {
            if (!(isa<LoadInst>(&I) || isa<StoreInst>(&I)
                            || isa<GetElementPtrInst>(&I)))
                continue;

            Builder.SetInsertPoint(&I);
            Value *Ptr = nullptr;
            Value *Type = Builder.getInt8(-1);
#define LOAD 0
#define STORE 1
#define GETELEMENTPTR 2
            if (LoadInst *LI = dyn_cast<LoadInst>(&I)) {
                Ptr = LI->getPointerOperand();
                Type = Builder.getInt8(LOAD);
            } else if (StoreInst *SI = dyn_cast<StoreInst>(&I)) {
                Ptr = SI->getPointerOperand();
                Type = Builder.getInt8(STORE);
            } else if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(&I)) {
                Ptr = GEP->getPointerOperand();
                Type = Builder.getInt8(GETELEMENTPTR);
            }
            Builder.CreateCall(HookFunc, {Ptr, Type});
        }
    }

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


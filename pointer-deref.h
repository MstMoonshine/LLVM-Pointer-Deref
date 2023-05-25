#pragma once

#include "llvm/IR/PassManager.h"

namespace llvm {

class PointerDerefPass : public PassInfoMixin<PointerDerefPass> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);
    static bool isRequired() { return true; }
};

}

#include "llvm/IR/PassManager.h"

struct IRdealer : public llvm::PassInfoMixin<IRdealer> {
	llvm::PreservedAnalyses run(llvm::Function &F, 
					llvm::FunctionAnalysisManager &FAM);
};


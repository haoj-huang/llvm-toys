
#include "llvm/IR/PassManager.h"

struct StrictOpt : public llvm::PassInfoMixin<StrictOpt> {
	llvm::PreservedAnalyses run(llvm::Function &F, 
					llvm::FunctionAnalysisManager &FAM);
};


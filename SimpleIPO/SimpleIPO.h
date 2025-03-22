
#include "llvm/IR/PassManager.h"

struct SimpleIPO : public llvm::PassInfoMixin<SimpleIPO> {
	llvm::PreservedAnalyses run(llvm::Module &M, 
					llvm::ModuleAnalysisManager &MAM);
};


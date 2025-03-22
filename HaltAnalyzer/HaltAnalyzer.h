
#include "llvm/IR/PassManager.h"

class HaltAnalyzer : public llvm::PassInfoMixin<HaltAnalyzer> {
	static constexpr const char* HaltFuncName = "my_halt";
	llvm::SmallVector<llvm::Instruction*, 2> Calls;
	void findHaltCalls(llvm::Function &F);
public:
	llvm::PreservedAnalyses run(llvm::Function &F, 
					llvm::FunctionAnalysisManager &FAM);
};


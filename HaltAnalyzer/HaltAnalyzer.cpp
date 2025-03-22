
#include "HaltAnalyzer.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/Dominators.h"

using namespace llvm;

void HaltAnalyzer::findHaltCalls(Function &F) {
	Calls.clear();
	for (auto &I : instructions(F)) {
		if (auto *CI = dyn_cast<CallInst>(&I)) {
			if (CI->getCalledFunction()->getName() == HaltFuncName)
				Calls.push_back(&I);
		}
	}
	return;
}

PreservedAnalyses HaltAnalyzer::run(Function &F, FunctionAnalysisManager &FAM) {
	findHaltCalls(F);
	DominatorTree &DT = FAM.getResult<DominatorTreeAnalysis>(F);
	SmallVector<BasicBlock*, 4> DomBBs;

	for (auto *I : Calls) {
		auto *BB = I->getParent();
		DomBBs.clear();
		DT.getDescendants(BB, DomBBs);
		for (auto *DomBB : DomBBs) {
			if (DomBB != BB) {
				DomBB->printAsOperand(errs() << "[WARNING] Unreachable BB: ");
				errs() << "\n";
			}
		}
	}

	return PreservedAnalyses::all();
}

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK 
llvmGetPassPluginInfo() {
	return { LLVM_PLUGIN_API_VERSION, "HaltAnalyzer", "v0.1", 
				[](PassBuilder &PB) { 
					PB.registerOptimizerLastEPCallback(
					[](ModulePassManager &MPM, 
					   PassBuilder::OptimizationLevel OL) {
						MPM.addPass(
						createModuleToFunctionPassAdaptor(HaltAnalyzer())); 
					}
					);
				}
			};	
}



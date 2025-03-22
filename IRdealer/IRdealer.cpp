
#include "IRdealer.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/CFG.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/SCCIterator.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"

using namespace llvm;

class MyInstVisitor : public InstVisitor<MyInstVisitor> {
public:
	void visitBinaryOperator(BinaryOperator &BOp) {
		errs() << BOp.getOpcodeName() << "\n";
	}

	void visitReturnInst(ReturnInst &RI) {
		errs() << "ret "; 
		RI.getReturnValue()->printAsOperand(errs());
		errs() << "\n";
		errs() << "ret Value: ";
	   	RI.getReturnValue()->print(errs());
		errs() << "\n";
	}

	void visitCallInst(CallInst &CI) {
		errs() << CI.getCalledFunction()->getName() << "\n";
	}
};

PreservedAnalyses IRdealer::run(Function &F, FunctionAnalysisManager &FAM) {
	
	MyInstVisitor vst;
	vst.visit(F);

	errs() << "===------------------===\n";
	BasicBlock &EntryBB = F.getEntryBlock();
	for (BasicBlock *BB : post_order(&EntryBB)) {
		BB->printAsOperand(errs());
		errs() << "\n";
	}	
	errs() << "=----------------------=\n";
	for (BasicBlock *BB : depth_first(&F)) {
		BB->printAsOperand(errs());
		errs() << "\n";
	}
	errs() << "===------------------===\n";
	for (auto SCCI = scc_begin(&F); !SCCI.isAtEnd(); ++SCCI) {
		const std::vector<BasicBlock*> SCC = *SCCI;
		for (auto *BB : SCC) {
			BB->printAsOperand(errs());
			errs() << "\n";
		}
		errs() << "=----=\n";
	}
	errs() << "===------------------===\n";

	return PreservedAnalyses::all();
}

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK 
llvmGetPassPluginInfo() {
	return { LLVM_PLUGIN_API_VERSION, "IRdealer", "v0.1", 
				[](PassBuilder &PB) { 
					
					PB.registerPipelineParsingCallback(
					[](StringRef Name, FunctionPassManager &FPM, 
							ArrayRef<PassBuilder::PipelineElement>) { 
						if (Name == "ir-dealer") {
							FPM.addPass(IRdealer());
							return true;
						}
						return false;
					}
					);
					/*
					PB.registerPipelineStartEPCallback(
					[](ModulePassManager &MPM, 
						PassBuilder::OptimizationLevel OL) {
						if (OL.getSpeedupLevel() >= 2) {
							MPM.addPass(
							createModuleToFunctionPassAdaptor(IRdealer())); 
						}
					}
					);*/
				}
			};	
}



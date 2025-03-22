
#include "SimpleIPO.h"
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
#include "llvm/Support/Debug.h"

#define DEBUG_TYPE "simple-ipo"

using namespace llvm;

PreservedAnalyses SimpleIPO::run(Module &M, ModuleAnalysisManager &MAM) {
	CallGraph CG(M);
	LLVM_DEBUG(CG.print(errs()));
	LLVM_DEBUG(dbgs() << "===----------------===\n");
	for (auto &Node : CG) {
		if (Node.first) {
			LLVM_DEBUG(dbgs() << Node.first->getName() << "\n");
		}
	}
	LLVM_DEBUG(dbgs() << "===----------------===\n");
	return PreservedAnalyses::all();
}

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK 
llvmGetPassPluginInfo() {
	return { LLVM_PLUGIN_API_VERSION, "SimpleIPO", "v0.1", 
				[](PassBuilder &PB) { 
					
					PB.registerPipelineParsingCallback(
					[](StringRef Name, ModulePassManager &MPM, 
							ArrayRef<PassBuilder::PipelineElement>) { 
						if (Name == "simple-ipo") {
							MPM.addPass(SimpleIPO());
							return true;
						}
						return false;
					}
					);
					/*
					PB.registerOptimizerLastEPCallback(
					[](ModulePassManager &MPM, 
						PassBuilder::OptimizationLevel OL) {
						MPM.addPass(SimpleIPO()); 
					}
					);*/
				}
			};	
}



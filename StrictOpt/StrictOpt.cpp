
#include "StrictOpt.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"

using namespace llvm;

PreservedAnalyses StrictOpt::run(Function &F, FunctionAnalysisManager &FAM) {
	bool Modified = false;
	
	errs() << "==================\n";
	if (F.hasSection()) {
		errs() << "section " << F.getSection() << "\n";
	} else {
		errs() << "no section, set one: test_section\n";
		F.setSection("test_section");
	}
	errs() << "==================\n";

	for (auto &Arg : F.args()) {
		if (Arg.getType()->isPointerTy() && 
			!Arg.hasAttribute(Attribute::NoAlias)) {
			Arg.addAttr(Attribute::NoAlias);
			Modified |= true;
		}
	}

	auto PA = PreservedAnalyses::all();
	if (Modified)
		PA.abandon<AAManager>();
	return PA;
}

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK 
llvmGetPassPluginInfo() {
	return { LLVM_PLUGIN_API_VERSION, "StrictOpt", "v0.1", 
				[](PassBuilder &PB) { 
					/*
					PB.registerPipelineParsingCallback(
					[](StringRef Name, FunctionPassManager &FPM, 
							ArrayRef<PassBuilder::PipelineElement>) { 
						if (Name == "strict-opt") {
							FPM.addPass(StrictOpt());
							return true;
						}
						return false;
					}
					);*/
					
					PB.registerPipelineStartEPCallback(
					[](ModulePassManager &MPM, 
						PassBuilder::OptimizationLevel OL) {
						if (OL.getSpeedupLevel() >= 2) {
							MPM.addPass(
							createModuleToFunctionPassAdaptor(StrictOpt())); 
						}
					}
					);
				}
			};	
}



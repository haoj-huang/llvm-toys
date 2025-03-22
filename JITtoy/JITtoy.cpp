
//#include "llvm/ExecutionEngine/MCJIT.h"
//#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/Core.h"
#include "llvm-c/ExecutionEngine.h"
//#include "llvm/IR/LLVMContext.h"
//#include "llvm/IR/Module.h"
//#include "llvm/Support/MemoryBuffer.h"
//#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
//#include "llvm/Support/FileSystem.h"

using namespace llvm;

int main(){
	LLVMInitializeNativeTarget();
	LLVMInitializeNativeAsmPrinter();
	LLVMInitializeNativeAsmParser();
	LLVMLinkInMCJIT();

	const char * Path = "/opt/test/sum.bc";
	LLVMMemoryBufferRef Buffer;
	char * msg = NULL;
	
	LLVMBool Ret;
	if (Ret = LLVMCreateMemoryBufferWithContentsOfFile(
							Path, &Buffer, &msg)) {
		errs() << msg << ": " << Path << "\n";
		return Ret;
	}

	LLVMModuleRef M;
	if (Ret = LLVMParseBitcode(Buffer, &M, &msg)) {
		errs() << msg << "\n";
		return Ret;
	}

	LLVMExecutionEngineRef EE;
	if (Ret = LLVMCreateExecutionEngineForModule(&EE, M, &msg)) {
		errs() << msg << "\n";
		return Ret;
	}

	const char * FunctionName = "sum";
	uint64_t ptr = LLVMGetFunctionAddress(EE, FunctionName);
	if (!ptr) {
		errs() << "Unable to get pointer to sum from Module\n";
		return 2;
	}
	int (*Sum)(int, int) = (int(*)(int, int))ptr;
	int res = Sum(4, 5);
	outs() << "Sum(4, 5) result: " << res << "\n";
	res = Sum(res, 6);
	outs() << "Sum(res, 5) result: " << res << "\n";

	LLVMShutdown();

	return 0;
}


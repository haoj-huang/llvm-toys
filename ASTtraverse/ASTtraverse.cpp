
extern "C" {
#include "clang-c/Index.h"
}
#include "llvm/Support/CommandLine.h"
#include <iostream>

using namespace llvm;

static cl::opt<std::string> FileName(
				cl::Positional, 
				cl::desc("Input File"), 
				cl::Required);

//const char *FuncNames[] = {
//#include "FuncNames.txt"
//};

#define CHECK_AND_OUT(FUNCNAME)                                         \
		if (strcmp(FUNCNAME, Name) == 0) { 								\
			std::cout << clang_getCString(fName) << ":" <<				\
            	line << ":" << col << " declares: " << Name << "\n";	\
		}

enum CXChildVisitResult visitNode(
				CXCursor cursor, 
				CXCursor parent, 
				CXClientData data) {
	auto kind = clang_getCursorKind(cursor);
	if (kind == CXCursor_CXXMethod || 
		kind == CXCursor_FunctionDecl) {
		CXString name = clang_getCursorSpelling(cursor);
		CXSourceLocation loc = clang_getCursorLocation(cursor);
		CXString fName;
		unsigned line = 0, col = 0;
		clang_getPresumedLocation(loc, &fName, &line, &col);
		auto Name = clang_getCString(name);
#include "FuncNames_Macro.txt"
		//for (const auto * f : FuncNames) {
		//	if (strcmp(f, Name) == 0) {
		//		std::cout << clang_getCString(fName) << ":" << 
		//			line << ":" << col << " declares: " << Name << "\n";
		//	}
		//}
		//std::cout << clang_getCString(fName) << ":" << 
		//		line << ":" << col << " declares: " << 
		//		clang_getCString(name) << "\n";
		return CXChildVisit_Continue;
	}
	return CXChildVisit_Recurse;
}

int main(int argc, char** argv) {
	cl::ParseCommandLineOptions(argc, argv, "AST Traverse Example");
	CXIndex index = clang_createIndex(0, 0);
	const char *args[] = { 
		"-I/usr/include", 
		"-I." 
	};
	CXTranslationUnit translationUnit = clang_parseTranslationUnit(
		index, FileName.c_str(), args, 2, NULL, 0, CXTranslationUnit_None);
	CXCursor cursor = clang_getTranslationUnitCursor(translationUnit);
	clang_visitChildren(cursor, visitNode, NULL);
	clang_disposeTranslationUnit(translationUnit);
	clang_disposeIndex(index);
	return 0;
}


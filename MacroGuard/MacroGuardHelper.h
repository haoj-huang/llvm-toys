
#include "clang/Lex/Preprocessor.h"

struct MacroGuardHelper {
	using ArgsVec = llvm::SmallVector<const clang::IdentifierInfo*, 2>;
	ArgsVec ArgsEnclosed;
	ArgsVec& GetArgsEnclosed() { return ArgsEnclosed; } 
};


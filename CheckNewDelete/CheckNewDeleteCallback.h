
#include "clang/ASTMatchers/ASTMatchFinder.h"

class MatchNewCallback : 
	public clang::ast_matchers::MatchFinder::MatchCallback {
	bool ReportError = false;
	llvm::StringRef NewExprTag;
	void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
public:
	explicit MatchNewCallback(bool B, llvm::StringRef S) : 
		ReportError(B), NewExprTag(S) {}
};

class MatchDeleteCallback : 
	public clang::ast_matchers::MatchFinder::MatchCallback {
	bool ReportError = false;
	llvm::StringRef DeleteExprTag;
	void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
public:
	explicit MatchDeleteCallback(bool B, llvm::StringRef S) : 
		ReportError(B), DeleteExprTag(S) {}
};



#include "clang/ASTMatchers/ASTMatchFinder.h"

class MatchAssignmentCallback : 
	public clang::ast_matchers::MatchFinder::MatchCallback {
	void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
};

class MatchReturnCallback : 
	public clang::ast_matchers::MatchFinder::MatchCallback {
	void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
};


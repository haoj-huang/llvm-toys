
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "TernaryConverterCallback.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::ast_matchers::internal;

DeclarationMatcher buildIfStmtMatcher(
				StatementMatcher truePattern, 
				StatementMatcher falsePattern) {
	return functionDecl(hasBody(compoundStmt(
				hasAnySubstatement(
					ifStmt(
						allOf(
							hasThen(truePattern),
							hasElse(falsePattern)
						)
					).bind("if_stmt")
				)
			)));
}

StatementMatcher buildReturnMatcher(StringRef Suffix) {
	auto Tag = ("return" + Suffix).str();
	return compoundStmt(
				statementCountIs(1), 
				hasAnySubstatement(
					returnStmt(hasReturnValue(expr().bind(Tag)))
				)
		   );
}

StatementMatcher buildAssignmentMatcher(StringRef Suffix) {
	auto DestTag = ("dest" + Suffix).str();
	auto ValTag = ("val" + Suffix).str();
	return compoundStmt(
				statementCountIs(1), 
				hasAnySubstatement(
					binaryOperator(
						hasOperatorName("="), 
						hasLHS(declRefExpr().bind(DestTag)), 
						hasRHS(expr().bind(ValTag))
					)
				)
		   );
}

struct TernaryConverterAction : public PluginASTAction {
	bool NoAssignment = false;
	bool NoReturn = false;
	
	std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override;

	ActionType getActionType() override { return Cmdline; }

	bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &Args) override {
		for (const auto &Arg : Args) {
			if (Arg == "-no-detect-assignment") NoAssignment = true;
			if (Arg == "-no-detect-return") NoReturn = true;
		}
		return true;
	}

private:
	std::unique_ptr<MatchFinder> ASTFinder;
	std::unique_ptr<MatchFinder::MatchCallback> ReturnMatchCB;
	std::unique_ptr<MatchFinder::MatchCallback> AssignMatchCB;
};

std::unique_ptr<ASTConsumer>
TernaryConverterAction::CreateASTConsumer(CompilerInstance &CI, 
	StringRef InFile) {
	ASTFinder = std::make_unique<MatchFinder>();
	if (!NoReturn) {
		ReturnMatchCB = std::make_unique<MatchReturnCallback>();
		ASTFinder->addMatcher(
			traverse(TK_IgnoreUnlessSpelledInSource, 
						buildIfStmtMatcher(
							buildReturnMatcher(".true"), 
							buildReturnMatcher(".false")
						)
					), 
			ReturnMatchCB.get());
	}
	if (!NoAssignment) {
		AssignMatchCB = std::make_unique<MatchAssignmentCallback>();
		ASTFinder->addMatcher(
			traverse(TK_IgnoreUnlessSpelledInSource, 
						buildIfStmtMatcher(
							buildAssignmentMatcher(".true"), 
							buildAssignmentMatcher(".false")
						)
					), 
			AssignMatchCB.get());
	}
	return std::move(ASTFinder->newASTConsumer());
}

static FrontendPluginRegistry::Add<TernaryConverterAction>
	X("ternary-converter", "convert if to ternary operator");



#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include <algorithm>
#include "CheckNewDeleteCallback.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::ast_matchers::internal;

const char newExpr[] = "newExpr";
const char deleteExpr[] = "deleteExpr";

StatementMatcher buildNewMatcher() {
	return cxxNewExpr().bind(newExpr);
}

StatementMatcher buildDeleteMatcher() {
	return cxxDeleteExpr().bind(deleteExpr);
}

struct CheckNewDeleteAction : public PluginASTAction {
	bool ReportError = false;
	
	std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override;

	ActionType getActionType() override { return Cmdline; }

	bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &Args) override {
		auto It = std::find_if(Args.begin(), Args.end(), 
			[](const std::string &Arg) {  return Arg == "-new-delete-report-error"; });
		ReportError = It != Args.end();
		return true;
	}

private:
	std::unique_ptr<MatchFinder> ASTFinder;
	std::unique_ptr<MatchFinder::MatchCallback> NewMatchCB;
	std::unique_ptr<MatchFinder::MatchCallback> DeleteMatchCB;
};

std::unique_ptr<ASTConsumer>
CheckNewDeleteAction::CreateASTConsumer(CompilerInstance &CI, 
	StringRef InFile) {
	ASTFinder = std::make_unique<MatchFinder>();
	NewMatchCB = std::make_unique<MatchNewCallback>(ReportError, newExpr);
	ASTFinder->addMatcher(
		traverse(TK_IgnoreUnlessSpelledInSource, 
				 buildNewMatcher()
				), 
		NewMatchCB.get());
	DeleteMatchCB = std::make_unique<MatchDeleteCallback>(ReportError, deleteExpr);
	ASTFinder->addMatcher(
		traverse(TK_IgnoreUnlessSpelledInSource, 
				 buildDeleteMatcher()
				), 
		DeleteMatchCB.get());
	return std::move(ASTFinder->newASTConsumer());
}

static FrontendPluginRegistry::Add<CheckNewDeleteAction>
	X("check-new-delete", "check if new or delete used");


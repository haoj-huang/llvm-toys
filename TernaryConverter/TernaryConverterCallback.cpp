
#include "TernaryConverterCallback.h"

using namespace clang;
using namespace clang::ast_matchers;

void MatchAssignmentCallback::run(const MatchFinder::MatchResult &Result) {
	const auto &Nodes = Result.Nodes;
	const auto *DestTrue = Nodes.getNodeAs<DeclRefExpr>("dest.true");
	const auto *DestFalse = Nodes.getNodeAs<DeclRefExpr>("dest.false");

	if (DestTrue && DestFalse &&
		DestTrue->getDecl() == DestFalse->getDecl()) {
		auto &Diag = Result.Context->getDiagnostics();
		auto DiagWarnMain = Diag.getCustomDiagID(
					DiagnosticsEngine::Warning, 
					"this if statement can be converted to ternary operator:");
		auto DiagNoteTrueExpr = Diag.getCustomDiagID(
					DiagnosticsEngine::Note, 
					"with true expression being this:");
		auto DiagNoteFalseExpr = Diag.getCustomDiagID(
					DiagnosticsEngine::Note, 
					"with false expression being this:");
		
		const auto *If = Nodes.getNodeAs<IfStmt>("if_stmt");
		Diag.Report(If->getBeginLoc(), DiagWarnMain);

		const auto *TrueValExpr = Nodes.getNodeAs<Expr>("val.true");
		const auto *FalseValExpr = Nodes.getNodeAs<Expr>("val.false");
		Diag.Report(TrueValExpr->getBeginLoc(), DiagNoteTrueExpr);
		Diag.Report(FalseValExpr->getBeginLoc(), DiagNoteFalseExpr);
	}
}

void MatchReturnCallback::run(const MatchFinder::MatchResult &Result) {
	const auto &Nodes = Result.Nodes;
	const auto *DestTrue = Nodes.getNodeAs<Expr>("return.true");
	const auto *DestFalse = Nodes.getNodeAs<Expr>("return.false");

	auto &Diag = Result.Context->getDiagnostics();
	auto DiagWarnMain = Diag.getCustomDiagID(
				DiagnosticsEngine::Warning, 
				"this if statement can be converted to ternary operator:");
	auto DiagNoteTrueExpr = Diag.getCustomDiagID(
				DiagnosticsEngine::Note, 
				"with true expression being this:");
	auto DiagNoteFalseExpr = Diag.getCustomDiagID(
				DiagnosticsEngine::Note, 
				"with false expression being this:");
	
	const auto *If = Nodes.getNodeAs<IfStmt>("if_stmt");
	Diag.Report(If->getBeginLoc(), DiagWarnMain);
	Diag.Report(DestTrue->getBeginLoc(), DiagNoteTrueExpr);
	Diag.Report(DestFalse->getBeginLoc(), DiagNoteFalseExpr);	
}



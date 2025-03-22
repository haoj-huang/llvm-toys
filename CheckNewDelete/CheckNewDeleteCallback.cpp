
#include "CheckNewDeleteCallback.h"

using namespace clang;
using namespace clang::ast_matchers;

void MatchNewCallback::run(const MatchFinder::MatchResult &Result) {
	const auto &Nodes = Result.Nodes;
	const auto *NewExpr = Nodes.getNodeAs<CXXNewExpr>(NewExprTag);

	auto &Diag = Result.Context->getDiagnostics();
    auto DiagMain = Diag.getCustomDiagID(
					ReportError ? DiagnosticsEngine::Error : 
								  DiagnosticsEngine::Warning,
                    "new operator used here:");
	Diag.Report(NewExpr->getBeginLoc(), DiagMain);
}

void MatchDeleteCallback::run(const MatchFinder::MatchResult &Result) {
	const auto &Nodes = Result.Nodes;
	const auto *DeleteExpr = Nodes.getNodeAs<CXXDeleteExpr>(DeleteExprTag);

	auto &Diag = Result.Context->getDiagnostics();
    auto DiagMain = Diag.getCustomDiagID(
					ReportError ? DiagnosticsEngine::Error : 
								  DiagnosticsEngine::Warning,
                    "delete operator used here:");
	Diag.Report(DeleteExpr->getBeginLoc(), DiagMain);
}




#include "MacroGuardValidator.h"

using namespace clang;

void MacroGuardValidator::MacroDefined(const Token &MacroNameToken, 
				const MacroDirective *MD) {
	const MacroInfo *MI = MD->getMacroInfo();
	const auto & Args = Helper.GetArgsEnclosed();
	for (const IdentifierInfo *ArgII : Args) {
		for (auto TokId = 0U, TokSize = MI->getNumTokens();
			TokId < TokSize; ++TokId) {
			Token CurTok = *(MI->tokens_begin() + TokId);
			if (CurTok.getIdentifierInfo() == ArgII) {
				if (TokId > 0 && TokId < TokSize - 1) {
					auto PreTok = *(MI->tokens_begin() + TokId - 1);
					auto NextTok = *(MI->tokens_begin() + TokId + 1);
					if (PreTok.is(tok::l_paren) && NextTok.is(tok::r_paren))
						continue;
				}
				auto DiagWarnID = Diag.getCustomDiagID(
					DiagnosticsEngine::Warning, 
					"macro argument '%0' is not enclosed by parenthesis");
				Diag.Report(CurTok.getLocation(), DiagWarnID) 
						<< ArgII->getName();
			}
		}
	}
}


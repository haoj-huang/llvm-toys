
#include "clang/Lex/Pragma.h"
#include "clang/Lex/Preprocessor.h"
#include "MacroGuardValidator.h"

using namespace clang;

struct MacroGuardHandler : public PragmaHandler {
	bool IsValidatorRegistered;
	MacroGuardHelper Helper;
	MacroGuardHandler() : PragmaHandler("macro_arg_guard"), IsValidatorRegistered(false) {}
	void HandlePragma(Preprocessor &PP, PragmaIntroducer Introducer, Token &PragmaTok) override;
	MacroGuardHelper & GetHelper() { return Helper; }
};

void MacroGuardHandler::HandlePragma(Preprocessor &PP, 
									PragmaIntroducer Introducer, 
									Token &PragmaTok) {
	Token Tok;
	PP.Lex(Tok);
	auto &ArgsToEnclosed = Helper.GetArgsEnclosed();
	while (Tok.isNot(tok::eod)) {
		ArgsToEnclosed.push_back(Tok.getIdentifierInfo());
		PP.Lex(Tok);
	}

	if (!IsValidatorRegistered) {
		auto Validator = std::make_unique<MacroGuardValidator>(
						PP.getSourceManager(), 
						PP.getDiagnostics(), 
						Helper);
		PP.addPPCallbacks(std::move(Validator));
		IsValidatorRegistered = true;
	}
}

static PragmaHandlerRegistry::Add<MacroGuardHandler> 
	X("macro_arg_guard", "Verify if designated macro args are enclosed");


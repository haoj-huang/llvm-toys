
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/PPCallbacks.h"
#include "MacroGuardHelper.h"

class MacroGuardValidator : public clang::PPCallbacks {
	clang::SourceManager &SM;
	MacroGuardHelper &Helper;
	clang::DiagnosticsEngine &Diag;
public:
	explicit MacroGuardValidator(clang::SourceManager &SM, 
					clang::DiagnosticsEngine &Diag, 
					MacroGuardHelper &Helper) : 
			SM(SM), Diag(Diag), Helper(Helper) {}
	void MacroDefined(const clang::Token &MacroNameToken, const clang::MacroDirective *MD) override;
};


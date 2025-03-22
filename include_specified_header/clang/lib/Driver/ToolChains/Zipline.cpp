
#include "Zipline.h"
#include "CommonArgs.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Options.h"
#include "llvm/Option/ArgList.h"
#include "llvm/Support/FileSystem.h"

using namespace clang::driver;
using namespace clang::driver::toolchains;
using namespace llvm::opt;
using namespace clang;

namespace clang {
namespace driver {
namespace toolchains {

void ZiplineToolChain::AddClangSystemIncludeArgs(
				const ArgList &DriverArgs, 
				ArgStringList &CC1Args) const {
	Linux::AddClangSystemIncludeArgs(DriverArgs, CC1Args);
	
	SmallString<16> CustomIncludePath;
	llvm::sys::fs::expand_tilde("~/my_include", CustomIncludePath);
	
	addSystemInclude(DriverArgs, CC1Args, CustomIncludePath.c_str());
	return;	
}

}
}
}


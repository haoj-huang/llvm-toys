
#include "Linux.h"

namespace clang {
namespace driver {
namespace toolchains {

struct LLVM_LIBRARY_VISIBILITY ZiplineToolChain : 
		public Linux {
	
	ZiplineToolChain(const Driver &D, const llvm::Triple &Triple, 
					const llvm::opt::ArgList &Args) 
			: Linux(D, Triple, Args) {}

	~ZiplineToolChain() override {}

	bool IsIntegratedAssemblerDefault() const override { return false; }

	bool useIntegratedAs() const override { return false; }

	void AddClangSystemIncludeArgs(const llvm::opt::ArgList &DriverArgs, 
					llvm::opt::ArgStringList &CC1Args) const override;

private:
	//Tool *buildAssembler() const override;
	//Tool *buildLinker() const override;
};

}
}
}



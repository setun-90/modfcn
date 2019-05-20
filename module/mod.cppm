#include "clang/Basic/Diagnostic.h"
#include "clang/CodeGen/ModuleBuilder.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"



export module modfcn;

export class mod {
public:
	mod(char const *);

	void *monosym(char const *);

private:
	clang::DiagnosticsEngine           _dengine;
	clang::CodeGenerator               _generator;
	std::unique_ptr<llvm::orc::LLJIT>  _lengine;
};

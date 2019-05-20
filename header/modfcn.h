#ifndef   MODFCN_H
#define   MODFCN_H

#include <memory>
#include "clang/Serialization/PCHContainerOperations.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/DiagnosticIDs.h"
#include "clang/CodeGen/ModuleBuilder.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"



class mod {
public:
	mod(char const *);

	void *monosym(char const *);

private:
	clang::DiagnosticIDs               _dids;
	clang::DiagnosticOptions           _dopts;
	clang::DiagnosticsEngine           _dengine;
	std::unique_ptr<clang::ASTUnit>    _unit;
	clang::CodeGenerator               _generator;
	std::unique_ptr<llvm::Module>      _module;
	std::unique_ptr<llvm::LLVMContext> _lcontext;
	std::unique_ptr<llvm::orc::LLJIT>  _lengine;
};



#endif /* MODFCN_H */

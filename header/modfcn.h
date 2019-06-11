#ifndef   MODFCN_H
#define   MODFCN_H

#include <memory>
#include "clang/AST/DeclTemplate.h"
#include "clang/Serialization/PCHContainerOperations.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/DiagnosticIDs.h"
#include "clang/CodeGen/ModuleBuilder.h"
#include "clang/Sema/Template.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"



class mod {
public:
	mod(char const *);
	~mod();

	void *sym(char const *);

private:
	/* Clang(source-)-related entities */
	clang::DiagnosticIDs               _dids;
	clang::DiagnosticOptions           _dopts;
	clang::IgnoringDiagConsumer        _dcons;
	clang::DiagnosticsEngine           _dengine;

	/*** Module interface */
	std::unique_ptr<clang::ASTUnit>    _unit;

	llvm::LLVMContext                  _lcontext;  // ORDER DEPENDENCY
	clang::CodeGenerator              *_generator; // ORDER DEPENDENCY
	llvm::ExecutionEngine             *_lengine;


	/*** Module implementation */


};



#endif /* MODFCN_H */

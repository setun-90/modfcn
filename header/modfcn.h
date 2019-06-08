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

	void *monosym(char const *);
	template <typename type> type *polysym(char const *name) {
		clang::ASTContext &ast(_unit->getASTContext());

		// Lookup

		// Instanciation

		// Compilation JIT
		_generator->Initialize(ast);
		_generator->HandleTranslationUnit(ast);
		llvm::Module *m(_generator->ReleaseModule());
		if (!m)
			throw std::runtime_error("LLVM Module not created");
		_lengine->addModule(std::unique_ptr<llvm::Module>(m));
		return (type *)(_lengine->getGlobalValueAddress(name));
	}

private:
	/* Clang(source-)-related entities */
	clang::DiagnosticIDs               _dids;
	clang::DiagnosticOptions           _dopts;
	clang::IgnoringDiagConsumer        _dcons;
	clang::DiagnosticsEngine           _dengine;

	/*** Module interface */
	llvm::LLVMContext                  _lcontext;
	llvm::ExecutionEngine             *_lengine;

	std::unique_ptr<clang::ASTUnit>    _unit;

	clang::CodeGenerator              *_generator;

	/*** Module implementation */


};



#endif /* MODFCN_H */

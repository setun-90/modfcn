#include "modfcn.h"
#include <exception>
#include <iostream>
#include <string>
#include <memory>
#include "clang/Frontend/ASTUnit.h"
#include "clang/Basic/DiagnosticIDs.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/FileSystemOptions.h"
#include "clang/Basic/LLVM.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/TargetSelect.h"



using namespace std;

mod::mod(char const *path):
	_dengine(&_dids, &_dopts, &_dcons),
	_unit(clang::ASTUnit::LoadFromASTFile(
		path,
		clang::RawPCHContainerReader(),
		clang::ASTUnit::WhatToLoad::LoadASTOnly,
		&_dengine,
		clang::FileSystemOptions()
	))
	{

	llvm::InitializeNativeTarget();

	if (!_unit)
		throw runtime_error("ASTUnit not created");
	clang::ASTContext &c(_unit->getASTContext());

	_generator.Initialize(c);
	_generator.HandleTranslationUnit(c);

	llvm::Module *m(_generator.StartModule("dynlib",_lcontext));
	if (!m)
		throw runtime_error("LLVM Module not created");
	_lengine = llvm::EngineBuilder(unique_ptr<llvm::Module>(m)).create();
	if (!_lengine)
		throw runtime_error("JIT Engine not created");

}

void *mod::monosym(char const *name) {
	return (void *)(_lengine->getGlobalValueAddress(name));
}

/*
To be done...

auto mod::polysym(char const *name);
*/

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
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/TargetSelect.h"



using namespace std;

mod::mod(char const *path):
	_dengine(&_dids, &_dopts),
	_lcontext(std::make_unique<llvm::LLVMContext>()) {

	llvm::InitializeNativeTarget();

	llvm::orc::LLJITBuilder b;
	llvm::handleAllErrors(b.prepareForConstruction());

	llvm::Expected<std::unique_ptr<llvm::orc::LLJIT>> e(b.create());
	if (!e)
		llvm::handleAllErrors(e.takeError());

	_lengine = std::move(*e);

	clang::RawPCHContainerReader r;
	clang::FileSystemOptions f;
	_unit = clang::ASTUnit::LoadFromASTFile(
		path,
		r,
		clang::ASTUnit::WhatToLoad::LoadASTOnly,
		&_dengine,
		f
	);
	if (!_unit)
		throw runtime_error("ASTUnit not created");

	clang::ASTContext &c(_unit->getASTContext());

	_generator.Initialize(c);
	_generator.HandleTranslationUnit(c);

	llvm::handleAllErrors(_lengine->addIRModule(llvm::orc::ThreadSafeModule(std::unique_ptr<llvm::Module>(_generator.StartModule("plugin", *_lcontext)), std::move(_lcontext))));
}

void *mod::monosym(char const *name) {
	llvm::Expected<llvm::JITEvaluatedSymbol> e(_lengine->lookup(name));
	if (!e)
		llvm::handleAllErrors(e.takeError(),
			[](llvm::orc::JITSymbolNotFound const &) {
			}
		);
	return (void *)(e->getAddress());
}

/*
To be done...

auto mod::polysym(char const *name);
*/

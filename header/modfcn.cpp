#include "modfcn.h"
#include <exception>
#include <iostream>
#include <string>
#include <memory>
#include "clang/Frontend/ASTUnit.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "clang/Basic/DiagnosticIDs.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/CodeGenOptions.h"
#include "clang/Basic/FileSystemOptions.h"
#include "clang/Basic/LLVM.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"



using namespace std;

// Necessary because clang::ASTUnit::visitLocalTopLevelDecls expects a pointer to function
static bool _mod_visitor(clang::DeclContext const **context, clang::Decl const *d) {
	bool p(d->getKind() != clang::Decl::Kind::Export);
	if (!p) *context = d->getDeclContext();
	return p;
}

mod::mod(char const *path, char const *lib_path):
	_dengine(&_dids, &_dopts, &_dcons),
	_unit(clang::ASTUnit::LoadFromASTFile(
		path,
		clang::RawPCHContainerReader(),
		clang::ASTUnit::WhatToLoad::LoadASTOnly,
		&_dengine,
		clang::FileSystemOptions()
	)),
	_lcontext(make_unique<llvm::LLVMContext>()),
	_generator(clang::CreateLLVMCodeGen(
		_dengine,
		"dynlib",
		clang::HeaderSearchOptions(),
		clang::PreprocessorOptions(),
		clang::CodeGenOptions(),
		*_lcontext
	))
	{

	// Prerequisite initialization
	if (llvm::InitializeNativeTarget())
		throw runtime_error("Native target not initialized");

	// Module interface loading
	if (!_unit)
		throw runtime_error("ASTUnit not created");

	if (!_unit->isModuleFile())
		throw runtime_error("File is not a serialized module");

	// /// Retrieve export declaration
	clang::DeclContext *p(nullptr);
	if (_unit->visitLocalTopLevelDecls(&p, (bool (*)(void *, clang::Decl const *))_mod_visitor) || !p)
		throw runtime_error("Module export not found");

	// /// Retrieve exported declarations

	clang::ASTContext &ast(p->getParentASTContext());
	_generator->Initialize(ast);
	_generator->HandleTranslationUnit(ast);
	auto m(_generator->ReleaseModule());
	if (!m)
		throw runtime_error("LLVM Module not created");

	{
		llvm::orc::LLJITBuilder b;
		llvm::handleAllErrors(b.prepareForConstruction());
		auto e(b.create());
		if (!e)
			llvm::handleAllErrors(e.takeError());
		_lengine = move(*e);
	}
	llvm::handleAllErrors(_lengine->addIRModule(llvm::orc::ThreadSafeModule(unique_ptr<llvm::Module>(m),move(_lcontext))));

	// Module implementation loading
	/*
	auto e(llvm::object::ObjectFile::createObjectFile(lib_path));
	if (!e)
		llvm::handleAllErrors(e.takeError());
	_lengine->addObjectFile(*e);
	*/

}

mod::mod(char const *path):
	mod(path, nullptr) {}

mod::~mod() {
	delete _generator;
}

void *mod::sym(char const *name) {

	// Lookup

	// Instanciation

	// Compilation JIT
	//_lengine->addModule(std::unique_ptr<llvm::Module>(m));
	auto e(_lengine->lookup(name));
	if (!e)
		llvm::handleAllErrors(e.takeError());
	return (void *)(e->getAddress());
}

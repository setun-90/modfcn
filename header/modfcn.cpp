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
#include "llvm/Support/TargetSelect.h"



using namespace std;

static bool _mod_visitor(clang::DeclContext const **context, clang::Decl const *d) {
	bool p(d->getKind() != clang::Decl::Kind::Export);
	if (!p) *context = d->getDeclContext();
	return p;
}

mod::mod(char const *path):
	_dengine(&_dids, &_dopts, &_dcons),
	_unit(clang::ASTUnit::LoadFromASTFile(
		path,
		clang::RawPCHContainerReader(),
		clang::ASTUnit::WhatToLoad::LoadASTOnly,
		&_dengine,
		clang::FileSystemOptions()
	)),
	_lengine(llvm::EngineBuilder().create()),
	_generator(clang::CreateLLVMCodeGen(
		_dengine,
		"dynlib",
		clang::HeaderSearchOptions(),
		clang::PreprocessorOptions(),
		clang::CodeGenOptions(),
		_lcontext
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

	if (!_lengine)
		throw runtime_error("JIT Engine not created");

	// Module implementation loading
	/*
	llvm::Expected<llvm::object::OwningBinary<llvm::object::ObjectFile>> e(llvm::object::ObjectFile::createObjectFile(lib_path));
	if (!e)
		llvm::handleAllErrors(e.takeError());
	_lengine->addObjectFile(*e);
	*/

}

mod::~mod() {
	delete _generator;
	delete _lengine;
}

void *mod::monosym(char const *name) {
	return nullptr;
}

#include "modfcn.h"
#include <exception>
#include <iostream>
#include <string>
#include <memory>
#include "clang/AST/DeclLookups.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "clang/Basic/DiagnosticIDs.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/CodeGenOptions.h"
#include "clang/Basic/FileSystemOptions.h"
#include "clang/Basic/LLVM.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/Object/ObjectFile.h"



using namespace std;

// Necessary because clang::ASTUnit::visitLocalTopLevelDecls expects a pointer to function
static bool _mod_visitor(clang::DeclContext const **context, clang::Decl const *d) {
	bool p(d->getKind() != clang::Decl::Kind::Export);
	if (!p) *context = d->getDeclContext();
	return p;
}

// For making the llvm::Error class at least a bit more useful...
static string get_message(llvm::Error const &e) {
	string s;
	llvm::raw_string_ostream o(s);
	o << e;
	return o.str();
}

mod::mod(char const *path, char const *lib_path):
	_diag(&_dids, &_dopts, &_dcons),
	_unit(clang::ASTUnit::LoadFromASTFile(
		path,
		clang::RawPCHContainerReader(),
		clang::ASTUnit::WhatToLoad::LoadASTOnly,
		&_diag,
		clang::FileSystemOptions()
	)),
	_dctx(nullptr),
	_lctx(make_unique<llvm::LLVMContext>()),
	_generator(clang::CreateLLVMCodeGen(
		_diag,
		"dynlib",
		clang::HeaderSearchOptions(),
		clang::PreprocessorOptions(),
		clang::CodeGenOptions(),
		*_lctx
	))
	{

	// Prerequisite initialization
	if (llvm::InitializeNativeTarget())
		throw runtime_error("Native target not initialized");

	// Module interface loading
	if (!_unit)
		throw runtime_error("File not loaded");

	if (!_unit->isModuleFile())
		throw runtime_error("File is not a serialized module");

	// /// Retrieve export declaration
	if (_unit->visitLocalTopLevelDecls(&_dctx, (bool (*)(void *, clang::Decl const *))_mod_visitor) || !_dctx)
		throw runtime_error("Module export not found");

	{
		auto r(_dctx->lookups());
		auto l(r.begin()), lf(r.end());
		cout << "***Lookups:\n";
		while (l != lf) {
			cout << l.getLookupName().getAsString() << "\n";
			++l;
		}
	}
	{
		auto r(_dctx->decls());
		auto l(r.begin()), lf(r.end());
		llvm::raw_os_ostream s(cout);
		cout << "***Declarations:\n";
		while (l != lf) {
			l->print(s);
			s << "\n";
			++l;
		}
	}

	// /// Retrieve exported declarations

	clang::ASTContext &ast(_dctx->getParentASTContext());
	_generator->Initialize(ast);
	_generator->HandleTranslationUnit(ast);
	auto m(_generator->ReleaseModule());
	if (!m)
		throw runtime_error("LLVM Module not created");

	llvm::orc::LLJITBuilder b;
	{
		auto e(b.prepareForConstruction());
		if (e)
			throw runtime_error(get_message(e));
	}
	{
		auto e(b.create());
		if (!e)
			throw runtime_error(get_message(e.takeError()));
		_jit = move(*e);
	}
	{
		auto e(_jit->addIRModule(llvm::orc::ThreadSafeModule(unique_ptr<llvm::Module>(m),move(_lctx))));
		if (e)
			throw runtime_error(get_message(e));
	}

	// Module implementation loading

	if (!lib_path)
		return;

	auto e1(llvm::object::ObjectFile::createObjectFile(lib_path));
	if (!e1)
		throw runtime_error(get_message(e1.takeError()));
	auto e2(_jit->addObjectFile(move(llvm::MemoryBuffer::getMemBuffer(e1->getBinary()->getData()))));
	if (e2)
		throw runtime_error(get_message(e2));

}

mod::mod(char const *path):
	mod(path, nullptr) {}

mod::~mod() {
	delete _generator;
	delete _dctx;
}

void *mod::sym(char const *name) {

	// Lookup

	// Instanciation

	// Compilation JIT
	//_jit->addModule(std::unique_ptr<llvm::Module>(m));
	auto e(_jit->lookup(name));
	if (!e)
		throw runtime_error(get_message(e.takeError()));
	return (void *)(e->getAddress());
}

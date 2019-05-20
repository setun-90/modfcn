#include <exception>
#include <string>
#include <memory>
#include "clang/Serialization/PCHContainerOperations.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Basic/DiagnosticIDs.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/FileSystemOptions.h"
#include "clang/Basic/LLVM.h"
#include "clang/CodeGen/ModuleBuilder.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"



module modfcn;

using namespace std;

mod::mod(char const *file_name)
	:_dengine(llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(),llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions>()),
	 _lengine(std::move(llvm::orc::LLJITBuilder().create().get())) {
	_generator.Initialize(clang::ASTUnit::LoadFromASTFile(
		file_name,
		clang::RawPCHContainerReader(),
		clang::ASTUnit::WhatToLoad::LoadEverything,
		llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine>(&_dengine),
		clang::FileSystemOptions()
	)->getASTContext());
	_lengine->addIRModule(llvm::orc::ThreadSafeModule());
}

void *mod::monosym(char const *name) {
	return nullptr;
}

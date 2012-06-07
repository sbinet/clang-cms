//===--- ClangCheckers.h - Provides builtin checkers ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ]
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_STATICANALYZER_CMS_CMSEXCEPTION_H
#define LLVM_CLANG_STATICANALYZER_CMS_CMSEXCEPTION_H

#include "llvm/Support/Regex.h"
#include "clang/AST/Type.h"

using namespace clang;

namespace clangcms {

class CmsException {
public:
	CmsException() ;
	~CmsException();

	bool reportGlobalStaticForType( QualType const& t ) const;
private:
	typedef std::vector< llvm::Regex *> ExList;
	mutable ExList m_exceptions;
};

} 

#endif

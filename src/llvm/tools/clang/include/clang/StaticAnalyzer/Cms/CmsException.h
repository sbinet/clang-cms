//===--- ClangCheckers.h - Provides builtin checkers ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_STATICANALYZER_CMS_CMSEXCEPTION_H
#define LLVM_CLANG_STATICANALYZER_CMS_CMSEXCEPTION_H

#include "clang/StaticAnalyzer/Core/Checker.h"
#include "llvm/Support/Regex.h"
//#include <boost/regex.hpp>


namespace clang {
namespace ento {

class CmsException {
public:
	CmsException() ;
	~CmsException();

	bool reportGlobalStaticForType( QualType const& t ) const;
private:
	typedef std::vector< llvm::Regex *> ExList;
	mutable ExList m_exceptions;
};

} // end namespace ento
} // end namespace clang

#endif

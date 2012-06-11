//===--- ClangCheckers.h - Provides builtin checkers ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ] and Patrick Gartung
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_STATICANALYZER_CMS_SUPPORT_H
#define LLVM_CLANG_STATICANALYZER_CMS_SUPPORT_H

#include "llvm/Support/Regex.h"

#include "clang/AST/Type.h"
#include "clang/Basic/SourceManager.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"

using namespace clang;


namespace clangcms {

namespace support {


// The three cases
//
// const int var;
// int const& var;
// int const* var;
//
// have to be handled slightly different. This function implements the functionality to check
// for const qualifier for all of them.
//
inline bool isConst( QualType const& qt )
{
	if ( qt->isReferenceType() )
	{
		// remove only the surounding reference type
		return qt.getNonReferenceType().isConstQualified();
	}
	if ( qt->isPointerType() )
	{
		clang::PointerType const* pt = qt->getAs<PointerType>();
		return pt->getPointeeType().isConstQualified();
	}

	// regular type
	return qt.isConstQualified();
}

}
} 

#endif

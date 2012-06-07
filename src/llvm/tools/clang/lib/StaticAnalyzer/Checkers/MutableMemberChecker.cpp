//== MutableMemberChecker.cpp - Checks for mutable members --------------*- C++ -*--==//
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

#include <iostream>

#include "ClangSACheckers.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"

using namespace clang;
using namespace ento;

namespace {
class MutableMemberChecker : public Checker< check::ASTDecl<FieldDecl> > {
  mutable OwningPtr<BuiltinBug> BT;
/*  void reportBug(const char *Msg,
                 ProgramStateRef StateZero,
                 CheckerContext &C) const ;*/
public:
  void checkASTDecl(const FieldDecl *D,
                      AnalysisManager &Mgr,
                      BugReporter &BR) const;
};  
} // end anonymous namespace

void MutableMemberChecker::checkASTDecl(const FieldDecl *D,
                    AnalysisManager &Mgr,
                    BugReporter &BR) const
{

	QualType t =  D->getType();

	if ( D->isMutable() &&
			// I *think* this means it is member of a class ...
			 D->getDeclContext()->isRecord() )
	{
	  PathDiagnosticLocation DLoc =
	    PathDiagnosticLocation::createBegin(D, BR.getSourceManager());

	    std::string buf;
	    llvm::raw_string_ostream os(buf);
	    os << "Mutable member'" << *D << "' in class, might be thread-unsafe when accessing via a const handle.";

	    BR.EmitBasicReport(D, "Possibly Thread-Unsafe: Mutable member",
	    					"ThreadSafety",
	                       os.str(), DLoc);
	    return;
	}

}

void ento::registerMutableMemberChecker(CheckerManager &mgr) {
  mgr.registerChecker<MutableMemberChecker>();
}

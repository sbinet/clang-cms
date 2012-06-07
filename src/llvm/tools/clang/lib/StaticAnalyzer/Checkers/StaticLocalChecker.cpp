//== StaticLocalChecker.cpp - Checks for non-const static locals --------------*- C++ -*--==//
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
class StaticLocalChecker : public Checker< check::ASTDecl<VarDecl> > {
  mutable OwningPtr<BuiltinBug> BT;
/*  void reportBug(const char *Msg,
                 ProgramStateRef StateZero,
                 CheckerContext &C) const ;*/
public:
  void checkASTDecl(const VarDecl *D,
                      AnalysisManager &Mgr,
                      BugReporter &BR) const;
};  
} // end anonymous namespace

void StaticLocalChecker::checkASTDecl(const VarDecl *D,
                    AnalysisManager &Mgr,
                    BugReporter &BR) const
{
	QualType t =  D->getType();
	if ( D->isStaticLocal() && !t.isConstQualified())
	{
	  PathDiagnosticLocation DLoc =
	    PathDiagnosticLocation::createBegin(D, BR.getSourceManager());

	    std::string buf;
	    llvm::raw_string_ostream os(buf);
	    os << "Non-const variable '" << *D << "' is local static and might be thread-unsafe";

	    BR.EmitBasicReport(D, "Possibly Thread-Unsafe: non-const static local variable",
	    					"ThreadSafety",
	                       os.str(), DLoc);
	    return;
	}
}

void ento::registerStaticLocalChecker(CheckerManager &mgr) {
  mgr.registerChecker<StaticLocalChecker>();
}

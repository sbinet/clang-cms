//== GlobalStaticChecker.cpp - Checks for non-const global statics --------------*- C++ -*--==//
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
class GlobalStaticChecker : public Checker< check::ASTDecl<VarDecl> > {
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

void GlobalStaticChecker::checkASTDecl(const VarDecl *D,
                    AnalysisManager &Mgr,
                    BugReporter &BR) const
{

	QualType t =  D->getType();
	if ( (D->getStorageClass() == SC_Static) &&
			  !D->isStaticDataMember() &&
			  !D->isStaticLocal() &&
			 !t.isConstQualified())
	{
	  PathDiagnosticLocation DLoc =
	    PathDiagnosticLocation::createBegin(D, BR.getSourceManager());

	    std::string buf;
	    llvm::raw_string_ostream os(buf);
	    os << "Non-const variable '" << *D << "' is static and might be thread-unsafe";

	    BR.EmitBasicReport(D, "Possibly Thread-Unsafe: non-const static variable",
	    					"ThreadSafety",
	                       os.str(), DLoc);
	    return;
	}

}

void ento::registerGlobalStaticChecker(CheckerManager &mgr) {
  mgr.registerChecker<GlobalStaticChecker>();
}
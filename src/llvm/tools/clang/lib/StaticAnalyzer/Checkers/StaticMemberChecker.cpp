//== DivZeroChecker.cpp - Division by zero checker --------------*- C++ -*--==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This defines DivZeroChecker, a builtin check in ExprEngine that performs
// checks for division by zeros.
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
class StaticMemberChecker : public Checker< check::ASTDecl<VarDecl> > {
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

void StaticMemberChecker::checkASTDecl(const VarDecl *D,
                    AnalysisManager &Mgr,
                    BugReporter &BR) const
{

	//std::cout << D->getNameAsString() << std::endl;
/*
	const VarDecl * var_D = dynamic_cast< const VarDecl* > ( D );
	// static locals
	if ( var_D != NULL )
	{*/
	if ( D->isStaticLocal())
	{
	  PathDiagnosticLocation DLoc =
	    PathDiagnosticLocation::createBegin(D, BR.getSourceManager());

	    std::string buf;
	    llvm::raw_string_ostream os(buf);
	    os << "Variable '" << *D << "' is local static and might be thread-unsafe";

	    BR.EmitBasicReport(D, "Possibly Thread-Unsafe: static local variable",
	    					"ThreadSafety",
	                       os.str(), DLoc);
	    return;
	}
	/*}*/
/*
	// static global vars
	if ( ( D->getStorageClass() == SC_Static ) &&
		  D->isStaticDataMember() )
	{



	  PathDiagnosticLocation DLoc =
	    PathDiagnosticLocation::createBegin(D, BR.getSourceManager());

	    std::string buf;
	    llvm::raw_string_ostream os(buf);
	    os << "Variable '" << *D << "' is static non-const and might be thread-unsafe";

	    BR.EmitBasicReport(D, "Possibly Thread-Unsafe: static non-const variable",
	    					"ThreadSafety",
	                       os.str(), DLoc);
	    return;
	}*/

}

void ento::registerStaticMemberChecker(CheckerManager &mgr) {
  mgr.registerChecker<StaticMemberChecker>();
}

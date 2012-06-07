//== ConstCastChecker.cpp - Checks for const_cast<> --------------*- C++ -*--==//
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

class ConstCastChecker: public Checker<check::PreStmt<CXXConstCastExpr> > {
public:
	mutable OwningPtr<BugType> BT;
	void checkPreStmt(const CXXConstCastExpr *CE, CheckerContext &C) const;
};
} // end anonymous namespace

void ConstCastChecker::checkPreStmt(const CXXConstCastExpr *CE,
		CheckerContext &C) const
{
	if (ExplodedNode *errorNode = C.generateSink()) {
		if (!BT)
			BT.reset(
					new BugType("const_cast used",
							"ThreadSafety"));
		BugReport *R = new BugReport(*BT, "const_cast was used, this may result in thread-unsafe code.", errorNode);
		R->addRange(CE->getSourceRange());

		C.EmitReport(R);
	}

}

void ento::registerConstCastChecker(CheckerManager &mgr) {
	mgr.registerChecker<ConstCastChecker>();
}

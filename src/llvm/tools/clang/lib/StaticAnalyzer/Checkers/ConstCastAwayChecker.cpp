//== ConstCastAwayChecker.cpp - Checks for removed const qualfiers --------------*- C++ -*--==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Check in a generic way if an explicit cast removes a const qualifier.
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

class ConstCastAwayChecker: public Checker<check::PreStmt<ExplicitCastExpr> > {
public:
	mutable OwningPtr<BugType> BT;
	void checkPreStmt(const ExplicitCastExpr *CE, CheckerContext &C) const;
};

} // end anonymous namespace

void ConstCastAwayChecker::checkPreStmt(const ExplicitCastExpr *CE,
		CheckerContext &C) const {
	const Expr *E = CE->getSubExpr();
	ASTContext &Ctx = C.getASTContext();
	QualType OrigTy = Ctx.getCanonicalType(E->getType());
	QualType ToTy = Ctx.getCanonicalType(CE->getType());

	OrigTy.dump();
	ToTy.dump();

	if (OrigTy.isConstQualified() && !ToTy.isConstQualified()) {
		if (ExplodedNode *errorNode = C.generateSink()) {
			if (!BT)
				BT.reset(
						new BugType("const cast away",
								"ThreadSafety"));
			BugReport *R = new BugReport(*BT, "const qualifier was removed via a cast, this may result in thread-unsafe code.", errorNode);
			R->addRange(CE->getSourceRange());
			C.EmitReport(R);
		}
	}
}

void ento::registerConstCastAwayChecker(CheckerManager &mgr) {
	mgr.registerChecker<ConstCastAwayChecker>();
}

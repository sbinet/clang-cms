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
#include "clang/StaticAnalyzer/Cms/CmsException.h"

using namespace clang;
using namespace ento;

namespace clang {
namespace ento {

CmsException::CmsException()
{
	m_exceptions.push_back( new llvm::Regex( "edm::InputSourcePluginFactory::PMaker.*" ));
	m_exceptions.push_back( new llvm::Regex( "edm::InputSourcePluginFactory*" ));
}


CmsException::~CmsException()
{
	for ( ExList::iterator it = m_exceptions.begin();
			it != m_exceptions.end();
			++ it)
	{
		delete (*it);
	}
}


bool CmsException::reportGlobalStaticForType( QualType const& t ) const
{
	//std::string t.getAsString()
	for ( ExList::iterator it = m_exceptions.begin();
			it != m_exceptions.end();
			++ it)
	{
		std::string s = t.getAsString();
		StringRef str_r(s);
		if  ( (*it)->match( str_r ))
			return false;
	}

	return true;
}

}
}



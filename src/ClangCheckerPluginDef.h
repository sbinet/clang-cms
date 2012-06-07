#pragma once

#define DEF_CLANG_CHECKER_PLUGIN( CLASSNAME, NAME, DESCRIPTION ) \
extern "C" \
void clang_registerCheckers (CheckerRegistry &registry) { \
registry.addChecker<CLASSNAME>( NAME, DESCRIPTION );\
}\
\
extern "C"\
const char clang_analyzerAPIVersionString[] = CLANG_ANALYZER_API_VERSION_STRING;



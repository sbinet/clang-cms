# -*- python -*-

# imports ---------------------------------------------------------------------
import os
import os.path as osp

import waflib.Logs as msg

# functions -------------------------------------------------------------------

def pkg_deps(ctx):
    return

def options(ctx):
    ctx.load('find_llvm')
    return

def configure(ctx):

    ctx.load('find_llvm')
    ctx.find_llvm()
    ctx.find_libclang()

    ctx.check(features='cxx cxxprogram', lib="dl", uselib_store="dl")
    return

def build(ctx):
    ctx(
        features="cxx cxxshlib",
        name="ClangCms",
        source=[
            "src/ClangCmsCheckerPluginRegister.cpp",
            "src/CmsException.cpp",
            "src/ConstCastChecker.cpp",
            "src/ConstCastAwayChecker.cpp",
            "src/StaticLocalChecker.cpp",
            "src/GlobalStaticChecker.cpp",
            "src/MutableMemberChecker.cpp",
            ],
        target="ClangCms",
        use="LLVM clang",
        includes = "src",
        )
    return

## EOF ##
    

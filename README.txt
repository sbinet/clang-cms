
=== CLANG CMS ===

This are the CMS clang static analyzer checkers to crawl C/C++ code for constructs which my become problematic when running multi-threaded code or do violate CMS Framework Rules.

by Thomas Hauth - Thomas.Hauth@cern.ch

This is still in early development/evaluation phase. You are welcome to contribute your commens and source code changes.

== Available Checks == 

* Non-const local statics
  
  int foo()
  {
    static int myEvilLocalState;
  }

* Non-const global statics

  static g_myGlobalStatic;
  
* use of the mutable keyword ( breaks const-corrcetness )

  struct Foo{
    mutable int myEvilValue;
  };
  
* use of const_cast to remove const-ness

  std::string s = "23";
  std::string const& r_const = s;
  std::string & r = const_cast< std::string & >( r_const );

* every explicit cast statement that removes const-ness

  std::string s = "23";
  std::string const& r_const = s;
  std::string & r = (std::string &) ( r_const );  

Dedicated Checkers exist for each of this code constructs.

== Compile LLVM / clang with this extensions ==

Follow the directions to obtain and compile LLVM/clang here:

http://clang.llvm.org/get_started.html#build

Stick to the directory structure suggested by this website. Compile LLVM/clang and see if this is working. The root path of the LLVM subversion folder will in the following be aliased by <llvm_src>. The folder where you built llvm is aliased <llvm_bin>

Now, git clone the repository this the extentions to a location of your choice:
git clone https://hauth.web.cern.ch/hauth/git/clang_cms/

and copy the contents of the folder 
src/llvm/tools/clang/lib/StaticAnalyzer/Checkers
to <llvm_src>/tools/clang/lib/StaticAnalyzer/Checkers

and recompile clang. Done.
Export the path to the new clang binary ( Bash example ):
export PATH=<llvm_bin>/Debug+Asserts/bin/:$PATH

Run
<llvm_src>/tools/clang/tools/scan-build/scan-build

This will give you a list of all avaiable checkers and the ones of the category "threadsafety" are already listed there.

== Test on a small example (non-CMSSW) ==

Test out the newly compiled and modified clang, cd into the clang_cms/test folder and run:

<llvm_src>/tools/clang/tools/scan-build/scan-build -enable-checker threadsafety.ConstCast -enable-checker threadsafety.ConstCastAway -enable-checker threadsafety.GlobalStatic -enable-checker threadsafety.MutableMember -enable-checker threadsafety.StaticLocal make -B

This wil produce a clang static analyzer html your can open in your favorite browser. You can find the location in the output line, something along the lines:


scan-build: 6 bugs found.
scan-build: Run 'scan-view /tmp/scan-build-2012-04-26-13' to examine bug reports.

You then call:
> firefox /tmp/scan-build-2012-04-26-13/index.html
 

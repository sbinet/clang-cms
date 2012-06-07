
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

Export the path to the new clang binary ( Bash example ):
export PATH=<llvm_bin>/Debug+Asserts/bin/:$PATH

Now, git clone the repository this the extentions to a location of your choice:
git clone https://hauth.web.cern.ch/hauth/git/clang_cms/

Open the file CMakeLists.txt and modify the path to llvm/clang to fit for your build system. Then run these commands:

cmake .
make

The compiled plugin will be in lib/ and is named ClangCms.so

If you want to do short test-drive, go to the clang_cms/test folder and run:

clang++ -Xclang -load -Xclang <clang_cms>/lib/ClangCms.so -Xclang -analyzer-checker=threadsafety.GlobalStatic --analyze global_static.cpp

== Load the plugin in scan-build == 

Unfortunately, clang does not provide a way to load plugins during it's analyze run using regular make files (scan-build). To enable this, a patch must be applied to the clang analyzer script

cd <llvm_src>
patch -p0 -i <clang_cms>/scan-build.patch

This is a perl script, so there is no need for a recompile. Now you can set the plugins to load during the analyze run via:

export CCC_ANALYZER_PLUGINS="-load <clang_cms>/lib/ClangCms.so"

== Test on a small example (non-CMSSW) ==

Test out the newly compiled plugin and cd into the clang_cms/test folder and run:

<llvm_src>/tools/clang/tools/scan-build/scan-build -enable-checker threadsafety.ConstCast -enable-checker threadsafety.ConstCastAway -enable-checker threadsafety.GlobalStatic -enable-checker threadsafety.MutableMember -enable-checker threadsafety.StaticLocal make -B

This will produce a clang static analyzer html your can open in your favorite browser. You can find the location in the output line, something along the lines:

scan-build: 6 bugs found.
scan-build: Run 'scan-view /tmp/scan-build-2012-04-26-13' to examine bug reports.

You then call:
> firefox /tmp/scan-build-2012-04-26-13/index.html
 

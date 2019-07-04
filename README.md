WHAT
====

This is a project that collects together several Open Source sub-projects from the Darwin (OS X) opensource repository (http://www.opensource.apple.com/darwinsource/)

**NOTE**
  This is code has been modified, although based on the posted sources.
  problem reports should be filed to **https://github.com/iains/darwin-xtools/issues**
  **Please do not** file problem reports for this code to Apple.

The sub-projects are
 cctools ; providing as, nm, ar, ranlib, strip, etc.
 ld64 ; providing the static linker and some test utils.
 dyld ; actually only used for headers
 several stub libraries used when support is needed on older systems.

The principal changes common to all versions are:

1. Build with CMAKE
  - I wanted to avoid a clash with the build stuff contained in the original sources
  - This is the same approach as used for other Darwin/OS X toolchain content (i.e. LLVM)
2. Support for PPC/PPC64.
  - This is a heavily modified merge of the XCode 3.2.6 ppc support + my own additions to re-introduce ppc64.
  - The branch islanding code has been almost completely re-written and it now supports multiple code sections (which GCC emits) and works for larger binaries (e.g. a Debug version of clang).
3. Bug-fixes for GCC-support.
4. Support for the content that LLVM/Clang emit on earlier Darwin.
  - XCode tools <= 3.2.6 do not support the macosx-version-min load command in "ar, ranlib" etc.
  - This means Darwin9/10 (OS X 10.5/6) need updated tools to handle LLVM.

PRE-REQUISITES
==============

1. cmake - I've been using 3.4.1 without issue on Darwin9...Darwin14 (OS X 10.5 +)

2. **A C++11 compiler**
  - on Darwin9, 10 and probably 11 that means building a compiler...
  - [GCC-5.3](https://github.com/iains/darwin-gcc-5) works fine and can be built using the default XCode toolchain.

HOW TO BUILD
============

1. Checkout the relevant composite branch.

2. I usually use a script file like this:

cmake -C /path/to/script/file.cmake /path/to/source

Example for 10.6 -- you will need to modify this as relevant to your case

darwin-xtools-10-6.cmake:

```
====== 8< =======
# Install to here.
SET(CMAKE_INSTALL_PREFIX /where/you/want/it CACHE PATH "put it here")
SET(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING "build style")

SET(CMAKE_OSX_DEPLOYMENT_TARGET "10.6" CACHE STRING "OS X Version")

# Default install place, change if you put it somehwere different.
set(SDK_BASE /Developer/SDKs)
SET(CMAKE_OSX_SYSROOT ${SDK_BASE}/MacOSX10.6.sdk CACHE PATH "system SDK 2")
SET(LLVM_DEFAULT_TARGET_TRIPLE x86_64-apple-darwin10 CACHE STRING "system triple")

SET(compilers /path/to/c++11/compiler/bin)
SET(CMAKE_C_COMPILER   ${compilers}/gcc CACHE PATH "C")
SET(CMAKE_CXX_COMPILER ${compilers}/g++ CACHE PATH "C++")

SET(CMAKE_C_FLAGS    "-mmacosx-version-min=10.6 -pipe" CACHE STRING "c flags")
SET(CMAKE_CXX_FLAGS  "-mmacosx-version-min=10.6 -pipe" CACHE STRING "cxx flags")

# This leaves assertions on.
SET(CMAKE_C_FLAGS_MINSIZEREL  "-Os" CACHE STRING "c   opt flags")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "-Os" CACHE STRING "cxx opt flags")

# If we're building with my GCC, then avoid carrying around a shared lib.
set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++ " CACHE STRING "toolchain exe ldflags")
set(CMAKE_SHARED_LINKER_FLAGS "-static-libstdc++ " CACHE STRING "toolchain shlib ldflags")

# If your host is 64b.
set(XTOOLS_HOST_IS_64B YES CACHE BOOL "host native bitwidth")

set(XTOOLS_BUGURL "https://githb.com/iains/darwin-xtools/issues" CACHE STRING "bug url")
====== >8 =======
```

You can set these things on the cmake invocation line like
```
cmake -DCMAKE_C_COMPILER=/path/to/compiler ... etc.
```

(but note that, unlike autoconf-based builds, I've not found a way to retrieve the command line used to configure the package, so files can be a useful record).

See the cmake documentation for the version you're using.

3. make, make install.
 - at present there's not a usable test-suite (although using the resulting tools to build GCC/LLVM is a fairly good test).

Changes
=======

1.0.1
ld64 has been adjusted so that FDEs which correspond to 0-sized entities are ignored rather than asserting.  If an entity is 0-sized, then it can't be a call-site or participate in unwinding (given that aliasses are handled specifically).  I guess one could say that the assert from ld64 was alerting to a problem elsewhere; however GCC was (and unpatched versions will) producing 0-sized functions for the case that the function body was elided as unreachable.  There are probably better solutions to both, but this is a reasonable one for now.

1.0.2
libtool was asserting for 'libtool -static -o arc.a file1.o .. '
This seems to be an over-aggressive check in sprintf when FORTIFY\_SOURCE=2.
"Fixed" by setting FORTIFY\_SOURCE=1
If \_FORTIFY\_SOURCE is set to 1, with compiler optimization level >= 1, checks that shouldn't change the behavior of conforming programs are performed.
At the same time, enabled DEBUG for CMAKE\_BUILD\_STYLE=Debug

1.1.0 - May 2016.
Updated to cctools-877.8 and ld64-253.9 (from XCode 7.2 sources).

Improved the reporting for --version to include the xtools version (and any defined
PACKAGE_VERSION, for packagers).  Improved --help to include a bug-report-url if
one is provided.

Added --version and --help to all the installed tools that were missing them.  This
should help with configuration tests.

1.2.0 - October 2016
Updated to cctools-886 and ld64-264.3.102 (from XCode 7.3.1 sources).

======= Corresponding to the 8.x Xcode published sources.

2.1.0 - December 2016

Updated to cctools-895 and ld64-274.1 (from XCode 8.1 sources).
Using libLTO, llvm-dsymutil from LLVM-3.8.1

2.2.1 - September 2017

Updated to ld64-274.2 (from XCode 8.2.1 sources).
Include libtapi if it's available.

2.2.3 - April 2019

Fix OSO output for cases where the same source file is used to produce
multiple objects with conditional compilation (fixes the dsymutil warning
"object symbol file not found for symbol".

2.2.4 - June 2019

Handle "zippered" tapi files.

2.3.3 - July 2019

Updated to cctools-898 and ld64-278.4 (from XCode 8.3.3 sources).
Using libLTO, llvm-dsymutil from LLVM-7.1 and libtapi-2.0.1

======= Corresponding to the 9.x Xcode published sources.

3.1.0 - July 2019

updated to cctools-900 and ld64-302.3.1 (Xcode 9.1 sources)
Using libLTO, llvm-dsymutil from LLVM-7.1 and libtapi-2.0.1





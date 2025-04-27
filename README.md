# Darwin Xtools 0.7.0r0

## Overview

This is a project that collects together several Open Source sub-projects from the Darwin (OS X) opensource repository (http://www.opensource.apple.com/darwinsource/)

**Important NOTE**

  This is code has been modified, although based on the posted sources.
  problem reports should be filed to **https://github.com/iains/darwin-xtools/issues**

**Please do not** file problem reports for this code to Apple.

The sub-projects are

 * cctools ; providing as, nm, ar, ranlib, strip, etc.
 * ld64 ; providing the static linker and some test utils.
 * dyld ; actually only used for headers
 * several stub libraries used when support is needed on older systems.

## Support for 'legacy' MacOSX versions - 0.7.0r0

This is a special release specifically supporting MacOSX 10.5 through 10.8 where the system C++ runtime is C++98.

* The linker is based on ld64-236.3
* cctools are based on cctools-855.

There are several bug fixes, some for critical issues (especially those affecting powerpc and powerpc64).  If you want to build larger powerpc binaries then you will most likely need these.

This project can be built using C++98 on the target systems using Apple-gcc-4.2.1 (or Apple clang where needed).

It is also possible to build the toolset on modern x86_64 and aarch64 macOS (so that it can be used to provide the assembler and linker for cross-compilers).

NEW in this version is some support for using ctest to run the ld64 unit tests (see below)

This package does **not** include dsymutil which was not part of the Apple open source until the switch to use LLVM, for that you will need to build at least LLVM-7.1.1 (which needs a C++11 compiler).

## Building.

You will need cmake - at least 3.9.6 (which can be built using Apple-gcc-4.2.1)

It can be helpful to use a cmake script file to capture relevant options and then:

`cmake -C /path/to/script/file.cmake /path/to/source`

Here is an example for 10.5 installed on an x86_64 machine (so supporting 64b code and powerpc via Rosetta1).

```
====== 8< =======
# Build style:
SET(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING "build style")
#SET(CMAKE_BUILD_TYPE Debug CACHE STRING "build style")
SET(ENABLE_ASSERTIONS ON CACHE BOOL "assertions")

# Install prefix
SET(CMAKE_INSTALL_PREFIX /opt/iains/i686-apple-darwin9/xtools-070r0 CACHE PATH "put it here")

# Specify the compiler
set(tools /Developer/usr)
set(compilers ${tools}/bin)

# 32b build
SET(CMAKE_C_COMPILER   ${compilers}/gcc-4.2 CACHE PATH "C")
SET(CMAKE_C_FLAGS      " -U__STRICT_ANSI__ " CACHE STRING "c flags")

SET(CMAKE_CXX_COMPILER ${compilers}/g++-4.2 CACHE PATH "C++")
SET(CMAKE_CXX_FLAGS      " -std=gnu++98 -U__STRICT_ANSI__" CACHE STRING "cxx flags")

# We are building 32b versions here.
set(XTOOLS_HOST_IS_64B NO CACHE BOOL "host native bitwidth")
set(CCTOOLS_LD_CLASSIC YES CACHE BOOL "can't build this 64b")
set(XTOOLS_BUGURL "https://githb.com/iains/darwin-xtools/issues" CACHE STRING "bug url")
set(PACKAGE_VERSION "Darwin xtools-0.7.0p0" CACHE STRING "package")

set(DARWIN_PREFER_PUBLIC_SDK YES CACHE BOOL "avoid the internal ones")

# ------- testing

#set(COMPILER_FOR_ARM "" CACHE STRING "")
#set(COMPILER_FOR_ARM64 "" CACHE STRING "")
set(COMPILER_FOR_I386 "/Developer/usr/bin/gcc-4.2" CACHE STRING "")
set(COMPILER_FOR_X86_64 "/Developer/usr/bin/gcc-4.2" CACHE STRING "")
set(COMPILER_FOR_PPC "/Developer/usr/bin/gcc-4.2" CACHE STRING "")
set(COMPILER_FOR_PPC64 "/Developer/usr/bin/gcc-4.2" CACHE STRING "")

#set(SDK_FOR_ARM "" CACHE STRING "")
#set(SDK_FOR_ARM64 "" CACHE STRING "")
set(SDK_FOR_I386 "/Developer/SDKs/MacOSX10.5.sdk" CACHE STRING "")
set(SDK_FOR_X86_64 "/Developer/SDKs/MacOSX10.5.sdk" CACHE STRING "")
set(SDK_FOR_PPC "/Developer/SDKs/MacOSX10.5.sdk" CACHE STRING "")
set(SDK_FOR_PPC64 "/Developer/SDKs/MacOSX10.5.sdk" CACHE STRING "")
set(SDK_FOR_10_5 "/Developer/SDKs/MacOSX10.5.sdk" CACHE STRING "")

set(EXECUTE_ARM NO CACHE BOOL "")
set(EXECUTE_ARM64 NO CACHE BOOL "")
set(EXECUTE_I386 YES CACHE BOOL "")
set(EXECUTE_X86_64 YES CACHE BOOL "")
set(EXECUTE_PPC YES CACHE BOOL "")
set(EXECUTE_PPC64 NO CACHE BOOL "")

====== >8 =======
```

`make && make install.`

## Testing

NOTE: There are quite a few failing tests (even on MacOSX 10.8), however there is an option to run the tests using the system tools - and, actually, more tests fail with them so this seems a step forward.

You can run all the tests with:

`ctest`

You can also run the same tests using the Apple tools on the system:

`TEST_SYSTEM_TOOLS=1 ctest`

## Changes

0.7.0r0 April 2025

Initial release, this has one additional bug-fix to ld64 in addition to the prelease fixes (a fix to a memory management issue that caused install-name-overrides to fail).

0.7.0p0

Initial release of this legacy support.


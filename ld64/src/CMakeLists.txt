if(XTOOLS_LTO_SUPPORT)
  add_definitions(-DLTO_SUPPORT)
  set(LTO_SUPPORT_INFO "(with LLVM LTO support)")
endif()

if(XTOOLS_BUGURL)
  add_definitions(-DXTOOLS_BUGURL="${XTOOLS_BUGURL}")
endif()

if(PACKAGE_VERSION)
  add_definitions(-DPACKAGE_VERSION="${PACKAGE_VERSION}")
endif()

if(XTOOLS_VERSION)
  add_definitions(-DXTOOLS_VERSION="${XTOOLS_VERSION}")
endif()

#include(cmake/config-ix.cmake)

# We should figure these out somehow - but for now let's just set them to
# the same as the fallback defaults.

SET(DEFAULT_MACOSX_MIN_VERSION "10.5")
SET(DEFAULT_IPHONEOS_MIN_VERSION "6.0")

SET(SUPPORT_ARCH_ppc 1)
SET(SUPPORT_ARCH_ppc64 1)

SET(SUPPORT_ARCH_i386 1)
SET(SUPPORT_ARCH_x86_64 1)
SET(SUPPORT_ARCH_x86_64h 1)

SET(SUPPORT_ARCH_armv6 1)
SET(SUPPORT_ARCH_armv6m 1)
SET(SUPPORT_ARCH_armv7 1)
SET(SUPPORT_ARCH_armv7em 1)
SET(SUPPORT_ARCH_armv7m 1)
SET(SUPPORT_ARCH_armv7s 1)
SET(SUPPORT_ARCH_arm64 1)

SET(LD64_SUPPORTED_TARGETS "ppc ppc64 i386 x86_64 x86_64h armv6 armv6m armv7 armv7s armv7m armv7em arm64")
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/configure.h.in"
  "${CMAKE_BINARY_DIR}/ld64/configure.h"
  )

# add the binary tree to the search path for include files
# so that we will find configure.h
include_directories("${CMAKE_BINARY_DIR}/ld64")

# Set up flags.
# If we re-use components (e.g. libprunetrie) we need the libc++ stuff to
# match up.

if (NOT XTOOLS_HAS_64BATADD)
  add_definitions(-DNO_64B_ATOMIC)
endif()

set(LD64_CXX_FLAGS)
if(XTOOLS_C_HAS_FNOCOMMON_FLAG)
     set(LD64_CXX_FLAGS "-fno-common")
endif()
if(XTOOLS_CXX_HAS_STDCXX11_FLAG)
     set(LD64_CXX_FLAGS "${LD64_CXX_FLAGS} -std=c++11")
endif()
if(XTOOLS_CXX_HAS_STDLIB_FLAG)
     set(LD64_CXX_FLAGS "${LD64_CXX_FLAGS} -stdlib=libc++")
endif()

set(LD64_CXX_FLAGS "${LD64_CXX_FLAGS} -lxar")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LD64_CXX_FLAGS} ")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/abstraction")

add_subdirectory(ld)
add_subdirectory(other)

add_library(prunetrie STATIC other/PruneTrie.cpp)

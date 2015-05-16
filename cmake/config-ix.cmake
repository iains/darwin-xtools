message(STATUS "*top config-ix* CMAKE_SYSTEM : ${CMAKE_SYSTEM}")

#include(CheckIncludeFile)
#include(CheckIncludeFileCXX)
#include(CheckLibraryExists)
#include(CheckSymbolExists)
#include(CheckFunctionExists)
#include(CheckCXXSourceCompiles)
#include(TestBigEndian)
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

# Flags used in several places.
check_c_compiler_flag(-fPIC               XTOOLS_C_HAS_FPIC_FLAG)
check_c_compiler_flag(-Wno-deprecated     XTOOLS_C_HAS_WNODEPRECATED_FLAG)
check_c_compiler_flag(-fno-common         XTOOLS_C_HAS_FNOCOMMON_FLAG)

check_cxx_compiler_flag(-std=c++11        XTOOLS_CXX_HAS_STDCXX11_FLAG)
check_cxx_compiler_flag(-stdlib=libc++    XTOOLS_CXX_HAS_STDLIB_FLAG)

# clang ignores this but produces a compile "unused option" warning.
if (CMAKE_COMPILER_IS_GNUCXX)
  check_cxx_compiler_flag(-static-libstdc++ XTOOLS_CXX_HAS_STATICCXX_FLAG)
endif()



# Check for common headers/libs that we need.

message(STATUS "*top config-ix* CMAKE_SYSTEM : ${CMAKE_SYSTEM}")

include(CheckIncludeFile)
include(CheckIncludeFileCXX)
#include(CheckLibraryExists)
#include(CheckSymbolExists)
include(CheckFunctionExists)
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

if (XTOOLS_CXX_HAS_STDCXX11_FLAG)
  if(XTOOLS_CXX_HAS_STDLIB_FLAG)
    check_include_file_cxx(unordered_map     XTOOLS_CXX_HAS_UNORDERED_MAP "-std=c++11 -stdlib=libc++")
  else()
    check_include_file_cxx(unordered_map     XTOOLS_CXX_HAS_UNORDERED_MAP -std=c++11)
  endif()
endif()
if (NOT XTOOLS_CXX_HAS_UNORDERED_MAP)
  check_include_file_cxx(tr1/unordered_map XTOOLS_CXX_HAS_TR1_UNORDERED_MAP)
endif()

check_function_exists(strnlen              XTOOLS_HAS_STRNLEN)
check_function_exists(strndup              XTOOLS_HAS_STRNDUP)

if((NOT XTOOLS_HAS_STRNLEN) OR (NOT XTOOLS_HAS_STRNDUP))
  set(XTOOLS_NEEDS_STRLIB ON)
endif()

check_function_exists(OSAtomicAdd64 XTOOLS_HAS_64BATADD)

# Top Level CMake file for XTOOLS.

cmake_minimum_required(VERSION 2.8.12.2)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to MinSizeRel")
  set(CMAKE_BUILD_TYPE MinSizeRel)
  set(XTOOLS_ENABLE_ASSERTIONS 1)
endif()

if (POLICY CMP0051)
  # CMake 3.1 and higher include generator expressions of the form
  # $<TARGETLIB:obj> in the SOURCES property.  These need to be
  # stripped everywhere that access the SOURCES property, so we just
  # defer to the OLD behavior of not including generator expressions
  # in the output for now.
  cmake_policy(SET CMP0051 OLD)
endif()

if(CMAKE_VERSION VERSION_LESS 3.1.20141117)
  set(cmake_3_2_USES_TERMINAL)
else()
  set(cmake_3_2_USES_TERMINAL USES_TERMINAL)
endif()

project(XTOOLS)
include(CTest)

# Add path for custom modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
  )

# Keep some kind of project identifier.

set(XTOOLS_VERSION_MAJOR 2)
set(XTOOLS_VERSION_MINOR 0)
set(XTOOLS_VERSION_PATCH 0)
set(XTOOLS_VERSION_SUFFIX git)

set(XTOOLS_VERSION "${XTOOLS_VERSION_MAJOR}.${XTOOLS_VERSION_MINOR}.${XTOOLS_VERSION_PATCH}")

if (NOT PACKAGE_VERSION)
  set(PACKAGE_VERSION "unpackaged")
endif()
  
if( CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR AND NOT MSVC_IDE)
  message(FATAL_ERROR "In-source builds are not allowed. ")
endif()

string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
set(UNIX_LIBDIR_SUFFIX "" CACHE STRING "Define suffix of library directory name (32/64)" )

if(CMAKE_CROSSCOMPILING)
  message(STATUS "*Top Level* Cross compiling")
else()
  message(STATUS "*Top Level* native build")
endif()

if(XTOOLS_LTO_PATH AND
   EXISTS ${XTOOLS_LTO_PATH}/include/llvm-c/lto.h AND
   EXISTS ${XTOOLS_LTO_PATH}/lib/libLTO.dylib)
  configure_file(
                ${XTOOLS_LTO_PATH}/include/llvm-c/lto.h
                ${CMAKE_BINARY_DIR}/include/llvm-c/lto.h
                COPYONLY)
  configure_file(
                ${XTOOLS_LTO_PATH}/lib/libLTO.dylib
                ${CMAKE_BINARY_DIR}/lib/libLTO.dylib
                COPYONLY)
  option(XTOOLS_LTO_SUPPORT "Support LTO in cctools and ld64." ON)
  message(STATUS "*Top Level* WITH LTO")
else()
  option(XTOOLS_LTO_SUPPORT "Support LTO in cctools and ld64." OFF)
  message(STATUS "*Top Level* NO LTO")
endif()

SET(CMAKE_C_FLAGS_MINSIZEREL   "-Os")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
#SET(CMAKE_C_FLAGS_DEBUG "-Og")
#SET(CMAKE_CXX_FLAGS_DEBUG "-Og")

include(config-ix)

set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
if (APPLE)
  set(CMAKE_INSTALL_NAME_DIR "@rpath")
  set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
else(UNIX)
  if(NOT DEFINED CMAKE_INSTALL_RPATH)
    set(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib${UNIX_LIBDIR_SUFFIX}")
    if (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,origin")
      set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,origin")
    endif()
  endif(NOT DEFINED CMAKE_INSTALL_RPATH)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/archs)

#link_directories("${CMAKE_SOURCE_DIR}/host-libs")

# *sigh* ... re-working cctools to allow regular 'bool' is possible but quite
# a lot of fiddling.  For now it's simply easier to use two versions of the
# dyld headers, one local to cctools and the ld64 one copied from up-to-date
# dyld.  This include dir should NOT be used for cctools.
if( EXISTS ${CMAKE_SOURCE_DIR}/dyld/include/mach-o/dyld_priv.h)
  configure_file(
                ${CMAKE_SOURCE_DIR}/dyld/include/mach-o/dyld_priv.h
                ${CMAKE_BINARY_DIR}/dyld-include/mach-o/dyld_priv.h
                COPYONLY)
  configure_file(
                ${CMAKE_SOURCE_DIR}/dyld/include/mach-o/dyld.h
                ${CMAKE_BINARY_DIR}/dyld-include/mach-o/dyld.h
                COPYONLY)
  configure_file(
               ${CMAKE_SOURCE_DIR}/dyld/include/mach-o/dyld_images.h
                ${CMAKE_BINARY_DIR}/dyld-include/mach-o/dyld_images.h
                COPYONLY)
endif()

include_directories(BEFORE SYSTEM "${CMAKE_SOURCE_DIR}/macho-target-includes")
include_directories(BEFORE SYSTEM "${CMAKE_BINARY_DIR}/include")
include_directories(BEFORE SYSTEM "${CMAKE_BINARY_DIR}/host-includes")
if (EXISTS ${CMAKE_BINARY_DIR}/lib)
  link_directories("${CMAKE_BINARY_DIR}/lib")
endif()
if (EXISTS ${CMAKE_BINARY_DIR}/archs)
  link_directories("${CMAKE_BINARY_DIR}/archs")
endif()

if(XTOOLS_NEEDS_STRLIB)
  add_subdirectory(strlib)
  configure_file(
                ${CMAKE_SOURCE_DIR}/strlib/string.h
                ${CMAKE_BINARY_DIR}/host-includes/string.h
                COPYONLY)
endif()

# Darwin < 10 doesn't specify the uuid_string_t, provide it here.
configure_file(${CMAKE_SOURCE_DIR}/host-includes/uuid/uuid.h
               ${CMAKE_BINARY_DIR}/host-includes/uuid/uuid.h
               COPYONLY)

if (NOT XTOOLS_HAS_MODERNXAR)
  add_subdirectory(xar-additions)
  configure_file(
                ${CMAKE_SOURCE_DIR}/xar-additions/xar.h
                ${CMAKE_BINARY_DIR}/host-includes/xar/xar.h
                COPYONLY)
endif()

# Evaluate first so that we find out about libprunetrie.
if( EXISTS ${CMAKE_SOURCE_DIR}/ld64/CMakeLists.txt )
  add_subdirectory(ld64)
endif()

set(XTOOLS_HAS_LIBPRUNETRIE)
if( EXISTS ${CMAKE_SOURCE_DIR}/ld64/src/other/PruneTrie.cpp)
  # We need to do this here, because cctools and ld items both want it.
  #add_library(prunetrie ${CMAKE_SOURCE_DIR}/ld64/src/other/PruneTrie.cpp)
  #include_directories("${CMAKE_SOURCE_DIR}/ld64/src/abstraction")
  # put the header where we can find it.
  configure_file(
                ${CMAKE_SOURCE_DIR}/ld64/src/other/prune_trie.h
                ${CMAKE_BINARY_DIR}/include/mach-o/prune_trie.h
                COPYONLY)
  set(XTOOLS_HAS_LIBPRUNETRIE YES)
endif()

if( EXISTS ${CMAKE_SOURCE_DIR}/cctools/CMakeLists.txt )
  add_subdirectory(cctools)
endif()

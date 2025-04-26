# Top Level CMake file for XTOOLS.

cmake_minimum_required(VERSION 3.9.6)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to MinSizeRel")
  set(CMAKE_BUILD_TYPE MinSizeRel)
  set(XTOOLS_ENABLE_ASSERTIONS 1)
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

set(XTOOLS_VERSION_MAJOR 0)
set(XTOOLS_VERSION_MINOR 7)
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
  message(STATUS "*Top Level* Cross compiling for ${CMAKE_SYSTEM_VERSION}")
else()
  message(STATUS "*Top Level* native build version ${CMAKE_SYSTEM_VERSION}")
endif()

if(FALSE AND XTOOLS_LTO_PATH AND
   EXISTS ${XTOOLS_LTO_PATH}/include/llvm-c/lto.h AND
   EXISTS ${XTOOLS_LTO_PATH}/lib/libLTO.dylib)
  configure_file(
                ${XTOOLS_LTO_PATH}/include/llvm-c/lto.h
                ${CMAKE_BINARY_DIR}/lto-include/llvm-c/lto.h
                COPYONLY)
  configure_file(
                ${XTOOLS_LTO_PATH}/lib/libLTO.dylib
                ${CMAKE_BINARY_DIR}/lib/libLTO.dylib
                COPYONLY)
  option(XTOOLS_LTO_SUPPORT "Support LTO in cctools and ld64." ON)
  message(STATUS "*Top Level* WITH LTO")
else()
  option(XTOOLS_LTO_SUPPORT "Support LTO in cctools and ld64." OFF)
  message(STATUS "*Top Level* NO LTO in 0.7.0 at present")
endif()

# No tapi for this version.
if(FALSE)
if(XTOOLS_TAPI_PATH AND
   EXISTS ${XTOOLS_TAPI_PATH}/include/tapi AND
   EXISTS ${XTOOLS_TAPI_PATH}/lib/libtapi.dylib)
  configure_file(
                ${XTOOLS_TAPI_PATH}/lib/libtapi.dylib
                ${CMAKE_BINARY_DIR}/lib/libtapi.dylib
                COPYONLY)
  option(XTOOLS_TAPI_SUPPORT "Support TAPI in cctools and ld64." ON)
  message(STATUS "*Top Level* WITH TAPI")
else()
  option(XTOOLS_TAPI_SUPPORT "Support TAPI in cctools and ld64." OFF)
  message(STATUS "*Top Level* NO TAPI")
endif()
endif()

SET(CMAKE_C_FLAGS_MINSIZEREL   "-Os")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
#SET(CMAKE_C_FLAGS_DEBUG "-Og")
#SET(CMAKE_CXX_FLAGS_DEBUG "-Og")

include(config-ix)

# check for Darwin version stuff.
if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  # See if we chose a specific c++ runtime.
  if(NOT DEFINED XTOOLS_USES_LIBSTDCXX)
    if(CMAKE_SYSTEM_VERSION GREATER_EQUAL 12.0.0)
      set(XTOOLS_USES_LIBSTDCXX NO CACHE BOOL "default to libc++ here")
    else()
      set(XTOOLS_USES_LIBSTDCXX YES CACHE BOOL "default to libstdc++ here.")
    endif()
    if(XTOOLS_CXX_HAS_STDLIB_FLAG)
      message(STATUS "*Top Level* -stdlib= option is available but no choice specified")
      if(XTOOLS_CXX_HAS_STDCXX11_FLAG)
        message(STATUS "*Top Level* .. setting to -stdlib=libc++")
        set(XTOOLS_USES_LIBSTDCXX NO CACHE BOOL "use libc++")
      else()
        message(STATUS "*Top Level* .. setting to -stdlib=libstdc++")
        set(XTOOLS_USES_LIBSTDCXX YES CACHE BOOL "use libstdc++")
      endif()
    elseif(${XTOOLS_USES_LIBSTDCXX})
        message(STATUS "*Top Level* .. using -stdlib=libstdc++")
    else()
        message(STATUS "*Top Level* .. using -stdlib=libc++")
    endif()
  endif()
endif()

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

# No tapi for this version.
if(FALSE)
if(XTOOLS_TAPI_SUPPORT)
  include_directories(BEFORE SYSTEM "${XTOOLS_TAPI_PATH}/include")
  message(STATUS "*Top Level* including ${XTOOLS_TAPI_PATH}/include")
endif()
endif()

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

if(CMAKE_SYSTEM_VERSION LESS 10.0.0)
  # Darwin < 10 doesn't specify the uuid_string_t, provide it here.
  configure_file(${CMAKE_SOURCE_DIR}/host-includes/uuid/uuid.h
               ${CMAKE_BINARY_DIR}/host-includes/uuid/uuid.h
               COPYONLY)
endif()

if (FALSE)
# We do not need xar for this version.
if (NOT XTOOLS_HAS_MODERNXAR)
  add_subdirectory(xar-additions)
  configure_file(
                ${CMAKE_SOURCE_DIR}/xar-additions/xar.h
                ${CMAKE_BINARY_DIR}/host-includes/xar/xar.h
                COPYONLY)
endif()
endif()

configure_file(${CMAKE_SOURCE_DIR}/host-includes/allocate.h
               ${CMAKE_BINARY_DIR}/host-includes/allocate.h
               COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/host-includes/maxpathlen.h
               ${CMAKE_BINARY_DIR}/host-includes/maxpathlen.h
               COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/host-includes/private-extern.h
               ${CMAKE_BINARY_DIR}/host-includes/private-extern.h
               COPYONLY)

include(ExternalProject)
if(NOT XTOOLS_HAS_UUID)
  if(APPLE)
    message(STATUS "*Top Level* bad news ... we did not find uuid")
  else()
    message(STATUS "*Top Level* need to build uuid for ${triple}")
    ExternalProject_Add(libuuid
                        PREFIX ${CMAKE_BINARY_DIR}/libuuid
                        URL ${CMAKE_SOURCE_DIR}/archs/libuuid.tar.gz
                        CONFIGURE_COMMAND
                        ${CMAKE_BINARY_DIR}/libuuid/src/libuuid/configure
                        --prefix=${CMAKE_BINARY_DIR}/libuuid
                        --host=${triple} --disable-all-programs
                        --disable-shared --enable-libuuid
                        AR=${AR_FOR_TARGET} RANLIB=${RANLIB_FOR_TARGET} > conf.txt
                        BUILD_COMMAND make > bld.txt 2>&1
                        INSTALL_COMMAND make install > ins.txt 2>&1)
    include_directories(BEFORE SYSTEM "${CMAKE_BINARY_DIR}/libuuid/include")
    link_directories("${CMAKE_BINARY_DIR}/libuuid/lib")
    set (XTOOLS_HAS_UUID 1)
  endif()
endif()

if(NOT XTOOLS_HAS_SSL_MD5 AND NOT APPLE)
  message(STATUS "*Top Level* need to build openssl for ${triple} with config ${openssl_config}")
  ExternalProject_Add(openssl
                      PREFIX ${CMAKE_BINARY_DIR}/openssl
                        URL ${CMAKE_SOURCE_DIR}/archs/openssl-1.1.1l.tar.gz
                        CONFIGURE_COMMAND
                        ${CMAKE_BINARY_DIR}/openssl/src/openssl/Configure
                        --prefix=${CMAKE_BINARY_DIR}/openssl no-shared ${openssl_config}
                        AR=${AR_FOR_TARGET} RANLIB=${RANLIB_FOR_TARGET}
                        CC=${CMAKE_C_COMPILER} > conf.txt
                        BUILD_COMMAND make > bld.txt 2>&1
                        INSTALL_COMMAND make install > ins.txt 2>&1)
    include_directories(BEFORE SYSTEM "${CMAKE_BINARY_DIR}/openssl/include")
    link_directories("${CMAKE_BINARY_DIR}/openssl/lib")
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

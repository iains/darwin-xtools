
set(CCTOOLS_VERSION_NUM 900.0)

add_definitions(-DALLOW_64BIT_LEB_ON_32B_TARGET=1)

# We need this on in the libraries as well as the apps.
if(XTOOLS_LTO_SUPPORT)
  add_definitions(-DLTO_SUPPORT)
  set(LTO_SUPPORT_INFO "(with LLVM LTO support)")
endif()

string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)

if(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" )
  add_definitions(-DDEBUG)
endif()

if(XTOOLS_BUGURL)
  add_definitions(-DXTOOLS_BUGURL="${XTOOLS_BUGURL}")
endif()

if(PACKAGE_VERSION)
  add_definitions(-DPACKAGE_VERSION="${PACKAGE_VERSION}")
endif()

if (XTOOLS_VERSION)
  add_definitions(-DXTOOLS_VERSION="${XTOOLS_VERSION}")
endif()

option(CCTOOLS_EFITOOLS  "Build efitools as part of cctools" OFF)
option(CCTOOLS_LD_CLASSIC  "Build ld_classic as part of cctools" ON)

configure_file (
  "${CMAKE_SOURCE_DIR}/cctools/cctools_version.c.in"
  "${CMAKE_BINARY_DIR}/cctools/cctools_version.c"
  )

add_subdirectory(libstuff)
add_subdirectory(cbtlibs)

if( EXISTS ${CMAKE_SOURCE_DIR}/cctools/as/CMakeLists.txt )
  add_subdirectory(as)
endif()

if( EXISTS ${CMAKE_SOURCE_DIR}/cctools/ar/CMakeLists.txt )
  add_subdirectory(ar)
endif()

if( EXISTS ${CMAKE_SOURCE_DIR}/cctools/misc/CMakeLists.txt )
  add_subdirectory(misc)
endif()

if( EXISTS ${CMAKE_SOURCE_DIR}/cctools/otool/CMakeLists.txt )
  add_subdirectory(otool)
endif()

if (CCTOOLS_LD_CLASSIC)
  if( EXISTS ${CMAKE_SOURCE_DIR}/cctools/ld/CMakeLists.txt )
    add_subdirectory(ld)
  else()
    message(WARNING "Asked to build ld_classic, but there's no CMakeList for it.")
  endif()
endif()

if (CCTOOLS_EFITOOLS)
  if( EXISTS ${CMAKE_SOURCE_DIR}/cctools/efitools/CMakeLists.txt )
    add_subdirectory(efitools)
  else()
    message(WARNING "Asked to build efitools, but there's no CMakeList for them.")
  endif()
endif()

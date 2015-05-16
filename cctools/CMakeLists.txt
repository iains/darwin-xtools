
set(CCTOOLS_VERSION_NUM 862)

# We need this on in the libraries as well as the apps.
if(XTOOLS_LTO_SUPPORT)
  add_definitions(-DLTO_SUPORT)
  set(LTO_SUPPORT_INFO "(with LLVM LTO support)")
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

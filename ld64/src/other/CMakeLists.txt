
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/ld64/src/ld")

if(XTOOLS_LTO_SUPPORT)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lLTO")
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

add_definitions(-DLD_VERS="xtools-ld64-${LD64_VERSION_NUM}")

set(OBJDUMP_SOURCES
    ObjectDump.cpp
    ../ld/parsers/macho_relocatable_file.cpp
    ../ld/parsers/lto_file.cpp
    ../ld/debugline.c
    )

add_executable(ObjectDump  ${OBJDUMP_SOURCES})
if(NOT APPLE)
target_link_libraries(ObjectDump dl)
endif()
add_executable(dyldinfo  dyldinfo.cpp)
add_executable(machocheck  machochecker.cpp)
if(APPLE)
 add_executable(rebase  rebase.cpp)
else()
 message(WARNING "not building rebase on this platform")
endif()
add_executable(unwinddump  unwinddump.cpp)

if(APPLE)
install(TARGETS ObjectDump dyldinfo machocheck rebase unwinddump DESTINATION bin)
else()
install(TARGETS ObjectDump dyldinfo machocheck unwinddump DESTINATION bin)
endif()

add_test(ObjectDump_ok ${CMAKE_BINARY_DIR}/bin/ObjectDump --help)
add_test(dyldinfo_ok ${CMAKE_BINARY_DIR}/bin/dyldinfo --help)
add_test(machocheck_ok ${CMAKE_BINARY_DIR}/bin/machocheck --help)

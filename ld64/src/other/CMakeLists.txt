
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/ld64/src/ld")

if(XTOOLS_LTO_SUPPORT)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lLTO")
endif()

add_definitions(-DLD_VERS="xtools-ld64-${LD64_VERSION_NUM}")

set(OBJDUMP_SOURCES
    ObjectDump.cpp
    ../ld/parsers/macho_relocatable_file.cpp
    ../ld/parsers/lto_file.cpp
    ../ld/debugline.c
    )

add_executable(ObjectDump  ${OBJDUMP_SOURCES})
add_executable(dyldinfo  dyldinfo.cpp)
add_executable(machocheck  machochecker.cpp)
add_executable(rebase  rebase.cpp)
add_executable(unwinddump  unwinddump.cpp)

install(TARGETS ObjectDump dyldinfo machocheck rebase unwinddump DESTINATION bin)

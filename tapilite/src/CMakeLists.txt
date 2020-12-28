
set(TAPILITE_SOURCES
	LinkerInterfaceFile.cpp
	Version.cpp
)

add_library(tapilite ${TAPILITE_SOURCES})

include_directories("${CMAKE_SOURCE_DIR}/tapilite/include")
include_directories("${CMAKE_SOURCE_DIR}/ld64/src/abstraction")
# for configure.h
include_directories("${CMAKE_BINARY_DIR}/ld64")

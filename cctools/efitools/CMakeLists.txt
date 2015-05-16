
include_directories("${CMAKE_SOURCE_DIR}/cctools/include")
add_definitions(-D__private_extern__=__attribute__\(\(__visibility__\("hidden"\)\)\))

add_executable(makerelocs makerelocs.c)
target_link_libraries(makerelocs stuff)

if(XTOOLS_C_HAS_WNODEPRECATED_FLAG)
  # suppress the 'import is deprecated' blather.
  target_compile_options(makerelocs PRIVATE -Wno-deprecated)
endif()

add_executable(mtoc mtoc.c )
target_link_libraries(mtoc stuff)

if(XTOOLS_C_HAS_WNODEPRECATED_FLAG)
  # suppress the 'import is deprecated' blather.
  target_compile_options(mtoc PRIVATE -Wno-deprecated)
endif()

install(TARGETS mtoc
        DESTINATION bin
       )

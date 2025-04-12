
set(AR_SRCS
    ar.c
    append.c
    archive.c
    contents.c
    delete.c
    extract.c
    misc.c
    move.c 
    print.c
    replace.c 
    )

include_directories("${CMAKE_SOURCE_DIR}/cctools/include")
add_definitions(-D__private_extern__=__attribute__\(\(__visibility__\("hidden"\)\)\))

add_executable(ar ${AR_SRCS})
target_link_libraries(ar stuff)
if(XTOOLS_C_HAS_WNODEPRECATED_FLAG)
  # suppress the 'import is deprecated' blather.
  target_compile_options(ar PRIVATE -Wno-deprecated)
endif()

install(TARGETS ar
        DESTINATION bin
       )

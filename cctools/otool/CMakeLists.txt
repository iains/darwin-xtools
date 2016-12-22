set(OTOOL_SRCS
    main.c
    ofile_print.c
    ppc_disasm.c
    i386_disasm.c
    arm_disasm.c
    arm64_disasm.c
    m68k_disasm.c
    m88k_disasm.c
    i860_disasm.c
    hppa_disasm.c
    sparc_disasm.c
    print_objc.c
    print_objc2_32bit.c
    print_objc2_64bit.c
    print_bitcode.c
    dyld_bind_info.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../libstuff/ofile.c
    )

set(COFF_PRINT)
if(CCTOOLS_EFITOOLS)
  set(COFF_PRINT coff_print.c)
endif()

include_directories("${CMAKE_SOURCE_DIR}/cctools/include")
include_directories("${CMAKE_SOURCE_DIR}/cctools/otool")
add_definitions(-DOTOOL -D__private_extern__=__attribute__\(\(__visibility__\("hidden"\)\)\))

if(XTOOLS_C_HAS_WNODEPRECATED_FLAG)
  # suppress the 'import is deprecated' blather.
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated")
endif()

set_source_files_properties(dyld_bind_info.c PROPERTIES COMPILE_FLAGS -std=c99)

add_executable(otool ${OTOOL_SRCS} ${COFF_PRINT})
target_link_libraries(otool stuff)
set_target_properties(otool PROPERTIES LINKER_LANGUAGE CXX)

# Prefer to build tools without deps on installed dylibs.
# We actually only need cxa_demangle - so it doesn't matter which c++ lib.
# On later OS X  (where there are two) this will come from libc++abi either way.
if(XTOOLS_CXX_HAS_STATICCXX_FLAG)
  set_target_properties(otool PROPERTIES LINK_FLAGS -static-libstdc++)
endif()

#if(XTOOLS_LTO_SUPPORT)
#  set_target_properties(otool PROPERTIES COMPILE_FLAGS -DLTO_SUPPORT)
#endif()

install(TARGETS otool
        DESTINATION bin
       )

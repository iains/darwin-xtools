
set(LD_CLASSIC_SRCS
    ld.c pass1.c objects.c sections.c cstring_literals.c symbols.c
    fvmlibs.c layout.c specs.c pass2.c generic_reloc.c rld.c sets.c
    4byte_literals.c 8byte_literals.c literal_pointers.c dylibs.c
    indirect_sections.c mod_sections.c i860_reloc.c ppc_reloc.c
    m88k_reloc.c hppa_reloc.c sparc_reloc.c coalesced_sections.c uuid.c
    debugcompunit.c debugline.c arm_reloc.c
    )

include_directories("${CMAKE_SOURCE_DIR}/cctools/include")
add_definitions(-D__private_extern__=__attribute__\(\(__visibility__\("hidden"\)\)\))

add_executable(ld_classic ${LD_CLASSIC_SRCS})

# has to be built and linked 32bit for now.
if(XTOOLS_HOST_IS_64B)
  target_compile_options(ld_classic PRIVATE -m32)
  target_link_libraries(ld_classic -m32 stuff-32)
else()
  target_link_libraries(ld_classic stuff)
endif()

if(XTOOLS_C_HAS_WNODEPRECATED_FLAG)
  # suppress the 'import is deprecated' blather.
  target_compile_options(ld_classic PRIVATE -Wno-deprecated)
endif()

install(TARGETS ld_classic
        DESTINATION bin
       )

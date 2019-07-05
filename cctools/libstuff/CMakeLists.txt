
if (CCTOOLS_EFITOOLS)
  set(COFF_BYTESEX "coff_bytesex.c")
else()
  set(COFF_BYTESEX)
endif()

set(LIBSTUFF_SOURCES
   allocate.c
   errors.c
   arch.c
   execute.c
   bytesex.c
   rnd.c
   hppa.c
   swap_headers.c
   reloc.c
   version_number.c
   get_toc_byte_sex.c
   best_arch.c
   fatals.c
   arch_usage.c
   ofile.c
   ofile_error.c
   print.c
   set_arch_flag_name.c
   seg_addr_table.c
   dylib_table.c
   breakout.c
   writeout.c
   checkout.c
   fatal_arch.c
   ofile_get_word.c
   vm_flush_cache.c
   hash_string.c
   dylib_roots.c
   guess_short_name.c
   SymLoc.c
   get_arch_from_host.c
   crc32.c
   macosx_deployment_target.c
   symbol_list.c unix_standard_mode.c
   lto.c
   llvm.c
   ${COFF_BYTESEX}
   ${CMAKE_BINARY_DIR}/cctools/cctools_version.c
   )
#   paths_relative_to_exe.c

include_directories("${CMAKE_SOURCE_DIR}/cctools/include")

add_definitions(
 -D__private_extern__=__attribute__\(\(__visibility__\("hidden"\)\)\)
 -D_MACH_I386_THREAD_STATUS_FPSTATE_LEGACY_FIELD_NAMES_
 -D_ARCHITECTURE_I386_FPU_FPSTATE_LEGACY_FIELD_NAMES_
 -DIN_CCTOOLS
 )

if(XTOOLS_C_HAS_WNODEPRECATED_FLAG)
  # suppress the 'import is deprecated' blather.
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated")
endif()

add_library(stuff STATIC ${LIBSTUFF_SOURCES})
if(XTOOLS_C_HAS_FNOCOMMON_FLAG)
  set_target_properties(stuff PROPERTIES COMPILE_FLAGS "-fno-common")
endif()

if(XTOOLS_HOST_IS_64B AND CCTOOLS_LD_CLASSIC)
  # we have to build ld_classic as a 32b exe, it's never been converted for a 64b
  # host.
  add_library(stuff-32 STATIC ${LIBSTUFF_SOURCES})
  target_compile_options(stuff-32 PRIVATE -m32)
  if(XTOOLS_C_HAS_FNOCOMMON_FLAG)
    target_compile_options(stuff-32 PRIVATE -fno-common)
  endif()
endif()

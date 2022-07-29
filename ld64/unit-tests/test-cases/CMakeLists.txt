
set (archs)
if (COMPILER_FOR_I386)
 set (archs ${archs} "i386")
endif()
if (COMPILER_FOR_X86_64)
 set (archs ${archs} "x86_64")
endif()
if (COMPILER_FOR_PPC)
 set (archs ${archs} "ppc")
endif()
if (COMPILER_FOR_PPC64)
 set (archs ${archs} "ppc64")
endif()

set (all_arch_tests
     16-byte-alignment
     absolute-symbol
     alias-command-line
     alias-objects
     align-modulus
     allow-stack-execute
     allowable-client
     archive-basic
     archive-duplicate
     archive-force-load
     archive-image_info
     archive-init-order
     archive-ObjC
     archive-ObjC-unexported
     archive-order
     archive-r-ObjC
     archive-weak
     auto-arch
     bind_at_load
     blank-stubs
     bundle_loader
     cfstring-and-cstring
     cfstring-coalesce
     cfstring-utf16
     check-init-abs
     check-init-bind
     check-init-no-rebase
     coalesce_weak_def_in_dylib
     coalesce-force
     code-signed-object-file
     commons-alignment
     commons-coalesced-dead_strip
     commons-mixed
     commons-order
     cpu-sub-types
     cpu-sub-types-preference
     cstring-alt-segment
     cstring-custom-section
     cstring-empty-labeled
     cstring-labels
     custom-segment-layout
     data-in-code
     dead_strip
     dead_strip_dylibs
     dead_strip_section_attribute
     dead_strip-archive
     dead_strip-archive-duplicate-def
     dead_strip-archive-eh
     dead_strip-archive-global
     dead_strip-archive-weak
     dead_strip-archive-weak-override
     dead_strip-entry-archive
     dead_strip-init-archive
     dead_strip-initializers
     dead_strip-r_symbol_desc
     dead_strip-weak-coalesce
     dead_strippable_dylib
     demangle
     dependency-logging
     dso_handle
     dtrace-old-probes
     dtrace-static-probes
     dtrace-static-probes-coalescing
     duplicate_symbols
     dwarf-archive-all_load
     dwarf-debug-notes
     dwarf-debug-notes-r
     dwarf-debug-notes-uuid
     dwarf-ignore
     dwarf-strip
     dwarf-strip-objc
     dylib-aliases
     dylib-main
     dylib-re-export-cycle
     dylib-upward
     dylib_file
     dylib_file-missing
     dylib_init
     eh-coalescing
     eh-coalescing-no-labels
     eh-coalescing-r
     eh-strip-test
     eh-stripped-symbols
     eh_frame
     empty-dylib
     empty-object
     end-label
     exported-symbols-dead_strip
     exported-symbols-wildcards
     exported-symbols-wildcards-dead_strip
     exported_symbols_list-eol
     exported_symbols_list-hidden
     exported_symbols_list-r
     external-reloc-sorting
     fatal_warning
     filelist
     flat-dylib
     flat-indirect-undefines
     flat-main
     force-weak
     function-starts
     header-pad
     hello-world
     hidden-r
     implicit-common2
     implicit-common4
     implicit-common5
     implicit_dylib
     indirect-dylib
     indirect-path-search
     init-order
     install-name-override
     interposable_list
     kext-basic
     kext-undefined-export
     label-on-end-of-section
     label-on-end-of-section-order
     large-bss
     large-data
     late-link-error
     lazy-dylib
     lazy-dylib-objc
     linker-optimization-hints
     linker_options-framework
     linker_options-library
     literals-coalesce
     literals-coalesce-alignment
     literals-coalesce-alignment2
     literals-coalesce-alignment3
     literals-coalesce2
     literals-labels
     loader_path
     local-symbol-partial-stripping
     Lpath
     main-stripped
     merge_zero_fill_sections
     missing-option-args
     multiple-entry-points
     no-data-bundle
     no-dynamic-common
     no-object-symbols
     no-uuid
     no_zero_fill_sections
     non-lazy-r
     non-lazy-sections-r
     objc-abi
     objc-category-archive
     objc-category-debug-notes
     objc-category-optimize
     objc-category-optimize-load
     objc-category-warning
     objc-class-alias
     objc-exported_symbols_list
     objc-gc-checks
     objc-literal-pointers
     objc-literal-pointers-strip
     objc-properties
     objc-references
     objc-selector-coalescing
     objc-visibility
     operator-new
     order_file
     order_file-ans
     order_file-archive
     order_file-zero-fill
     pipelined-linking
     prebound-split-seg
     private-non-lazy
     re-export-and-use
     re-export-cases
     re-export-flag
     re-export-layers
     re-export-optimizations
     re-export-optimizations-indirect
     re-export-relative-paths
     re-export-symbol
     read-only-relocs
     rebase-basic
     reexport_symbols_list
     relocs-asm
     relocs-c
     relocs-c2
     relocs-literals
     relocs-literals2
     relocs-literals3
     relocs-neg-from-local
     relocs-objc
     sectcreate-dead_strip
     section-labels
     section-labels-zero-fill
     section-names-long
     segment-labels
     segment-order
     shared-cache-dylib
     slow-x86-stubs
     special-labels
     stabs-coalesce
     stabs-directory-slash
     stack_addr_no_size
     stack_addr_size
     stack_size_no_addr
     static-executable
     static-executable-pie
     static-executable-weak-defines
# not usable, static-strip
     strip-test2
     strip-test3
     strip_local
     stripped-indirect-symbol-table
     stub-generation
     stub-generation-weak
     switch-jump-table
     symbol-hiding-umbrella
     symbol-moving
     symbol-resolver-basic
     symbol-resolver-hidden
     tentative-and-archive
     tentative-and-archive-code
     tentative-and-dylib
     tentative-to-real
     tentative-to-real-hidden
     tentative-to-real-r
     umbrella-dylib
     undefined-dynamic-lookup
     unexported_symbols_list-r
     unstrippable-symbols
     utf16-nul
     visibility-warning
     visibility-warning-dylib-v-archive
     weak-def-auto-hide
     weak-def-flag
     weak-def-hidden-and-global
     weak-def-ordinal
     weak-force
     weak_dylib
     weak_import
     weak_import-addend
     weak_import-force
     weak_import-local
     weak_import-r
     weak_import-undefined
     weak_import2
     why_live
     zero-fill
     zero-fill2
     zero-fill3
    )

# Tests that run on a sub-set of archs only
set (archtests_i386
     allow_heap_execute
     compact-unwind-basic
     efi-basic
# FIXME: depends on the test compiler being clang.
#tlv-basic
#tlv-dead_strip
#tlv-dylib
#tlv-r
     )

set (archtests_x86_64
     compact-unwind-basic
     efi-basic
     got-elimination
#tlv-basic
#tlv-dead_strip
#tlv-dylib
#tlv-r
    )

# FIXME: also depends on the test compiler being clang.
if (XTOOLS_LTO_SUPPORT)
 set (archtests_x86_64 ${archtests_x86_64}
      lto-archive-dylib
      lto-dead_strip-all-hidden
      lto-dead_strip-coalesce
      lto-dead_strip-inline-asm
      lto-dead_strip-objc
      lto-dead_strip-some-hidden
      lto-dead_strip-tentative
      lto-dead_strip-unused
      lto-dylib-aliases
      lto-dylib-export_list
      lto-dynamic_export
      lto-llvm-options
      lto-objc-archive
      lto-objc-image-info
      lto-object_path
      lto-preload-pie
      lto-static-pie
      lto-weak-native-override
      lto-weak_import
      llvm-integration
   )
 set (archtests_i386 ${archtests_i386}
      lto-archive-dylib
      lto-dead_strip-all-hidden
      lto-dead_strip-coalesce
      lto-dead_strip-inline-asm
      lto-dead_strip-objc
      lto-dead_strip-some-hidden
      lto-dead_strip-tentative
      lto-dead_strip-unused
      lto-dylib-aliases
      lto-dylib-export_list
      lto-dynamic_export
      lto-llvm-options
      lto-objc-archive
      lto-objc-image-info
      lto-object_path
      lto-preload-pie
      lto-static-pie
      lto-weak-native-override
      lto-weak_import
      llvm-integration
   )
endif()

set (archtests_arm
     branch-distance
     branch-interworking
     branch-long
     thumb-blx
     thumb-pointer
     )

set (archtests_ppc
     branch-islands
     branch-islands-cross-section
     branch-islands-sym-offs
     )

set (archtests_ppc64
     branch-islands
     branch-islands-cross-section
     branch-islands-sym-offs
     page-zero-size-ppc64
     )

foreach(arch ${archs})

 set(runexes "")
 set(SDK "")
 if(${arch} STREQUAL "x86_64")
   set(C_COMPILER ${COMPILER_FOR_X86_64})
   set(CXX_COMPILER ${CXX_FOR_X86_64})
   if (SDK_FOR_X86_64)
     set(SDK ${SDK_FOR_X86_64})
   endif()
   if (EXECUTE_X86_64)
     set(runexes "YES")
   endif()
 endif()
 if(${arch} STREQUAL "i386")
   set(C_COMPILER ${COMPILER_FOR_I386})
   set(CXX_COMPILER ${CXX_FOR_I386})
   if (SDK_FOR_I386)
     set(SDK ${SDK_FOR_I386})
   endif()
   if (EXECUTE_I386)
     set(runexes "YES")
   endif()
 endif()
 if(${arch} STREQUAL "ppc")
   set(C_COMPILER ${COMPILER_FOR_PPC})
   set(CXX_COMPILER ${CXX_FOR_PPC})
   if (SDK_FOR_PPC)
     set(SDK ${SDK_FOR_PPC})
   endif()
   if (EXECUTE_PPC)
     set(runexes "YES")
   endif()
 endif()
 if(${arch} STREQUAL "ppc64")
   set(C_COMPILER ${COMPILER_FOR_PPC64})
   set(CXX_COMPILER ${CXX_FOR_PPC64})
   if (SDK_FOR_PPC64)
     set(SDK ${SDK_FOR_PPC64})
   endif()
   if (EXECUTE_PPC64)
     set(runexes "YES")
   endif()
 endif()

 foreach(test ${all_arch_tests})
   add_test(NAME ${test}-${arch}
            COMMAND make -C ${CMAKE_CURRENT_BINARY_DIR}/${test} CC=${C_COMPILER} CXX=${CXX_COMPILER} SDK=${SDK} ARCH=${arch} EXECUTE_OK=${runexes} clean all )
 endforeach()

 if(${arch} STREQUAL "i386")
   foreach(test ${archtests_i386})
     add_test(NAME ${test}-${arch}
              COMMAND make -C ${CMAKE_CURRENT_BINARY_DIR}/${test} CC=${C_COMPILER} CXX=${CXX_COMPILER} SDK=${SDK} ARCH=${arch} EXECUTE_OK=${runexes} clean all )
   endforeach()
  endif()
 if(${arch} STREQUAL "x86_64")
   foreach(test ${archtests_x86_64})
     add_test(NAME ${test}-${arch}
              COMMAND make -C ${CMAKE_CURRENT_BINARY_DIR}/${test} CC=${C_COMPILER} CXX=${CXX_COMPILER} SDK=${SDK} ARCH=${arch} EXECUTE_OK=${runexes} clean all )
   endforeach()
  endif()
 if(${arch} STREQUAL "ppc")
   foreach(test ${archtests_ppc})
     add_test(NAME ${test}-${arch}
              COMMAND make -C ${CMAKE_CURRENT_BINARY_DIR}/${test} CC=${C_COMPILER} CXX=${CXX_COMPILER} SDK=${SDK} ARCH=${arch} EXECUTE_OK=${runexes} clean all )
   endforeach()
  endif()
 if(${arch} STREQUAL "ppc64")
   foreach(test ${archtests_ppc64})
     add_test(NAME ${test}-${arch}
              COMMAND make -C ${CMAKE_CURRENT_BINARY_DIR}/${test} CC=${C_COMPILER} CXX=${CXX_COMPILER} SDK=${SDK} ARCH=${arch} EXECUTE_OK=${runexes} clean all )
   endforeach()
  endif()
endforeach()

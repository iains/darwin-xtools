
include_directories("${CMAKE_SOURCE_DIR}/cctools/include")
add_definitions(-D__private_extern__=__attribute__\(\(__visibility__\("hidden"\)\)\))

if(XTOOLS_C_HAS_WNODEPRECATED_FLAG)
  # suppress the 'import is deprecated' blather.
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated")
endif()

if(XTOOLS_LTO_SUPPORT)
add_definitions(-DLTO_SUPORT)
endif()

add_executable(lipo lipo.c)
target_link_libraries(lipo stuff)

add_executable(size size.c)
target_link_libraries(size stuff)

add_executable(strings strings.c)
target_link_libraries(strings stuff)

add_executable(nm nm.c)
target_link_libraries(nm stuff)

# The same executable does ranlib
add_executable(libtool libtool.c)
target_link_libraries(libtool stuff)

add_executable(install_name_tool install_name_tool.c)
target_link_libraries(install_name_tool stuff)

add_executable(codesign_allocate codesign_allocate.c)
target_link_libraries(codesign_allocate stuff)

add_executable(nmedit strip.c)
target_link_libraries(nmedit stuff)
set_target_properties(nmedit PROPERTIES COMPILE_FLAGS "-DNMEDIT")

add_executable(checksyms checksyms.c)
target_link_libraries(checksyms stuff)

add_executable(indr indr.c)
target_link_libraries(indr stuff)

add_executable(segedit segedit.c)
target_link_libraries(segedit stuff)

add_executable(cmpdylib cmpdylib.c)
target_link_libraries(cmpdylib stuff)

add_executable(pagestuff pagestuff.c)
target_link_libraries(pagestuff stuff)

add_executable(redo_prebinding redo_prebinding.c)
target_link_libraries(redo_prebinding stuff)

add_executable(seg_addr_table seg_addr_table.c)
target_link_libraries(seg_addr_table stuff)

add_executable(check_dylib check_dylib.c)
target_link_libraries(check_dylib stuff)

add_executable(seg_hack seg_hack.c)
target_link_libraries(seg_hack stuff)

add_executable(ctf_insert ctf_insert.c)
target_link_libraries(ctf_insert stuff)

add_executable(strip strip.c)
if(XTOOLS_HAS_LIBPRUNETRIE)
  set_target_properties(strip PROPERTIES COMPILE_FLAGS "-DTRIE_SUPPORT")
  set_target_properties(strip PROPERTIES LINKER_LANGUAGE CXX)
  set(STRIP_FLAGS)
  if(XTOOLS_CXX_HAS_STDCXX11_FLAG)
     set(STRIP_FLAGS "-std=c++11")
  endif()
  if(XTOOLS_CXX_HAS_STDLIB_FLAG)
     set(STRIP_FLAGS "${STRIP_FLAGS} -stdlib=libc++")
  elseif(XTOOLS_CXX_HAS_STATICCXX_FLAG)
     if(STRIP_FLAGS)
       set(STRIP_FLAGS "${STRIP_FLAGS} -static-libstdc++")
     else()
       set(STRIP_FLAGS "-static-libstdc++")
     endif()
  endif()
  target_link_libraries(strip stuff prunetrie ${STRIP_FLAGS})
else()
  target_link_libraries(strip stuff)
endif()

install(TARGETS 
        lipo size strings nm libtool install_name_tool nmedit strip
        codesign_allocate checksyms indr segedit cmpdylib pagestuff
        redo_prebinding seg_addr_table check_dylib seg_hack ctf_insert
        DESTINATION bin
       )


include_directories("${CMAKE_SOURCE_DIR}/cctools/include")
add_definitions(-D__private_extern__=__attribute__\(\(__visibility__\("hidden"\)\)\) -DIN_CCTOOLS)

if(XTOOLS_C_HAS_WNODEPRECATED_FLAG)
  # suppress the 'import is deprecated' blather.
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated")
endif()

add_library(syminfo libsyminfo.c)
if(XTOOLS_C_HAS_FNOCOMMON_FLAG)
  set_target_properties(syminfo PROPERTIES COMPILE_FLAGS "-fno-common")
endif()
target_link_libraries(syminfo stuff)

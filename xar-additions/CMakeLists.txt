
set(XAREXTRALIB_SRC
 xar-additions.c
)

add_library(xarextralib STATIC ${XAREXTRALIB_SRC})
if(XTOOLS_C_HAS_FNOCOMMON_FLAG)
  set_target_properties(xarextralib PROPERTIES COMPILE_FLAGS "-fno-common")
endif()


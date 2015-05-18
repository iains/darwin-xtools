
add_definitions(-D__FBSDID\\\(x\\\)= )
set(STRLIB_SRC)
if(NOT XTOOLS_HAS_STRNLEN)
  list(APPEND STRLIB_SRC strnlen.c)
endif()
if(NOT XTOOLS_HAS_STRNDUP)
  list(APPEND STRLIB_SRC  strndup.c)
endif()

add_library(strlib STATIC ${STRLIB_SRC})
if(XTOOLS_C_HAS_FNOCOMMON_FLAG)
  set_target_properties(strlib PROPERTIES COMPILE_FLAGS "-fno-common")
endif()


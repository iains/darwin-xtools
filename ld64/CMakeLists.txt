
set(LD64_VERSION_NUM 236.3.0)
set(LD64_VERSION_DBL 236.3)

include_directories("${CMAKE_BINARY_DIR}/dyld-include")
add_subdirectory(src)
if( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/CMakeLists.txt )
  add_subdirectory(unit-tests)
endif()

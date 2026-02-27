
set(TAPI_FULL_VERSION "2.0.3" CACHE STRING "Specify tapi version.")
message(STATUS "TAPI version: ${TAPI_FULL_VERSION}")

set(TAPI_APPLE_VERSION "1000.10.8" CACHE STRING "Specify tapi version.")
message(STATUS "APPLE Tapi version: ${TAPI_APPLE_VERSION}")

set(TAPI_REPOSITORY_STRING "https://github.com/grobian/darwin-xtools.git" CACHE STRING
  "Vendor-specific text for showing the repository the source is taken from.")

if(TAPI_REPOSITORY_STRING)
  add_definitions(-DTAPI_REPOSITORY_STRING="${TAPI_REPOSITORY_STRING}")
endif()

set(TAPI_VENDOR "tapilite" CACHE STRING
  "Vendor-specific text for showing with version information.")

if (TAPI_VENDOR)
  add_definitions( -DTAPI_VENDOR="${TAPI_VENDOR} ")
endif()

if (TAPI_APPLE_VERSION)
  add_definitions( -DAPPLE_VERSION="${TAPI_APPLE_VERSION}")
endif ()

set(TAPI_CXX_FLAGS)
if(XTOOLS_C_HAS_FNOCOMMON_FLAG)
     set(TAPI_CXX_FLAGS "-fno-common")
endif()
if(XTOOLS_CXX_HAS_STDCXX11_FLAG)
     set(TAPI_CXX_FLAGS "${TAPI_CXX_FLAGS} -std=c++11")
endif()
if(XTOOLS_CXX_HAS_STDLIB_FLAG)
     set(TAPI_CXX_FLAGS "${TAPI_CXX_FLAGS} -stdlib=libc++")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TAPI_CXX_FLAGS} ")

add_subdirectory(src)

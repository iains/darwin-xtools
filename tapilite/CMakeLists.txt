
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

add_subdirectory(src)

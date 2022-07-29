message(STATUS "copying unit tests")
file (COPY test-cases DESTINATION . PATTERN CMakeLists.txt EXCLUDE)
message(STATUS "copying unit tests done")

add_subdirectory(include)
add_subdirectory(test-cases)

file (COPY include/test.h DESTINATION ./include )

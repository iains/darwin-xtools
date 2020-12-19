
set(AS_COMMON_SRCS
    app.c
    as.c
    atof-generic.c
    atof-ieee.c
    expr.c
    fixes.c
    flonum-const.c
    flonum-copy.c
    flonum-mult.c
    frags.c
    hash.c
    hex-value.c
    input-file.c
    input-scrub.c
    layout.c
    messages.c
    obstack.c
    read.c
    sections.c
    symbols.c
    write_object.c
    xmalloc.c
    dwarf2dbg.c
    )

set(X86_SRCS i386.c ${AS_COMMON_SRCS})

set(PPC_SRCS ppc.c ${AS_COMMON_SRCS})

set(ARM_SRCS arm.c ${AS_COMMON_SRCS})

include_directories("${CMAKE_SOURCE_DIR}/cctools/include")
include_directories("${CMAKE_SOURCE_DIR}/cctools/as")
include_directories("${CMAKE_SOURCE_DIR}/cctools/include/gnu")
add_definitions(-DNeXT_MOD -D__private_extern__=__attribute__\(\(__visibility__\("hidden"\)\)\))

if(XTOOLS_C_HAS_WNODEPRECATED_FLAG)
  # suppress the 'import is deprecated' blather.
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated")
endif()

add_executable(as-driver driver.c)
set_target_properties(as-driver PROPERTIES COMPILE_FLAGS "-Di486 -Di586 -Di686 -UPPC")
target_link_libraries(as-driver stuff)
set_target_properties(as-driver PROPERTIES OUTPUT_NAME as)
install(TARGETS as-driver DESTINATION bin )

add_executable(i386-as ${X86_SRCS})
set_target_properties(i386-as PROPERTIES COMPILE_FLAGS "-DI386 -Di486 -Di586 -Di686 -UPPC")
set_target_properties(i386-as PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/i386)
set_target_properties(i386-as PROPERTIES OUTPUT_NAME as)
target_link_libraries(i386-as stuff)
if(XTOOLS_NEEDS_STRLIB)
  target_link_libraries(i386-as strlib)
endif()
install(TARGETS i386-as DESTINATION bin/i386)

if(BUILD_TESTING)
  add_executable(test-x86 i386-check.c)
  set_target_properties(test-x86 PROPERTIES COMPILE_FLAGS "-DI386 -Di486 -Di586 -Di686 -UPPC")
  set_target_properties(test-x86 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
  add_test(NAME check-x86-as COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/check-as $<TARGET_FILE:test-x86> i386 $<TARGET_FILE:as-driver>)
endif()

add_executable(x8664-as ${X86_SRCS})
set_target_properties(x8664-as PROPERTIES COMPILE_FLAGS "-DI386 -Di486 -Di586 -Di686 -DARCH64 -UPPC")
set_target_properties(x8664-as PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/x86_64)
set_target_properties(x8664-as PROPERTIES OUTPUT_NAME as)
target_link_libraries(x8664-as stuff)
if(XTOOLS_NEEDS_STRLIB)
  target_link_libraries(x8664-as strlib)
endif()
install(TARGETS x8664-as DESTINATION bin/x86_64)

if(BUILD_TESTING)
  add_executable(test-x86_64 i386-check.c)
  set_target_properties(test-x86_64 PROPERTIES COMPILE_FLAGS "-DI386 -Di486 -Di586 -Di686 -DARCH64 -UPPC")
  set_target_properties(test-x86_64 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
  add_test(NAME check-x86_86-as COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/check-as $<TARGET_FILE:test-x86_64> x86_64 $<TARGET_FILE:as-driver>)
endif()

add_executable(ppc-as ${PPC_SRCS})
set_target_properties(ppc-as PROPERTIES COMPILE_FLAGS "-DPPC")
set_target_properties(ppc-as PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ppc)
set_target_properties(ppc-as PROPERTIES OUTPUT_NAME as)
target_link_libraries(ppc-as stuff)
if(XTOOLS_NEEDS_STRLIB)
  target_link_libraries(ppc-as strlib)
endif()
install(TARGETS ppc-as DESTINATION bin/ppc)

if(BUILD_TESTING)
  add_executable(test-ppc ppc-check.c)
  set_target_properties(test-ppc PROPERTIES COMPILE_FLAGS "-DPPC")
  set_target_properties(test-ppc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
  add_test(NAME check-ppc-as COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/check-as $<TARGET_FILE:test-ppc> ppc $<TARGET_FILE:as-driver>)
endif()

add_executable(ppc64-as ${PPC_SRCS})
set_target_properties(ppc64-as PROPERTIES COMPILE_FLAGS "-DPPC -DARCH64")
set_target_properties(ppc64-as PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ppc64)
set_target_properties(ppc64-as PROPERTIES OUTPUT_NAME as)
target_link_libraries(ppc64-as stuff)
if(XTOOLS_NEEDS_STRLIB)
  target_link_libraries(ppc64-as strlib)
endif()
install(TARGETS ppc64-as DESTINATION bin/ppc64)

add_executable(arm-as ${ARM_SRCS})
set_target_properties(arm-as PROPERTIES COMPILE_FLAGS "-DARM -UPPC")
set_target_properties(arm-as PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/arm)
set_target_properties(arm-as PROPERTIES OUTPUT_NAME as)
target_link_libraries(arm-as stuff)
if(XTOOLS_NEEDS_STRLIB)
  target_link_libraries(arm-as strlib)
endif()
install(TARGETS arm-as DESTINATION bin/arm)


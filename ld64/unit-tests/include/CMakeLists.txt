# We generate a Makefile include in the working dir so that
# we do not need to edit every source makefile.

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/common.makefile.in"
  "${CMAKE_CURRENT_BINARY_DIR}/common.makefile"
  )

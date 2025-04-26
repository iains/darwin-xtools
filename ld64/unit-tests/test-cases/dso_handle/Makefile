
TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

SHELL = bash # use bash shell so we can redirect just stderr

#
# Verify the linker errors out nicely when __dso_handle is defined in source.
#

run: all

all:
	${CC} ${LINKFLAGS} test.c -o test-${ARCH}
	${FAIL_IF_BAD_MACHO} test-${ARCH}
	${CC} ${LINKFLAGS} test.c -DDSO_DEF=1 -o test-def-${ARCH} 2>warnings.txt
	${FAIL_IF_BAD_MACHO} test-def-${ARCH}
	${CC} ${LINKFLAGS} test.c -DDSO_TENT=1 -o test-tent-${ARCH} -Wl,-w
	${PASS_IFF_GOOD_MACHO} test-tent-${ARCH}


clean:
	rm -f test-${ARCH} test-def-${ARCH} test-tent-${ARCH} warnings.txt

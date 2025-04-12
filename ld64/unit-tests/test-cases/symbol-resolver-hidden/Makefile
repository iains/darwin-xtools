
TESTROOT = ../..
MINOS = 10.6
include ${TESTROOT}/include/common.makefile

#
# Test a simple symbol resolver function
#
all: all-${ARCH}

# needs 10.6 and not supported for ppc
all-ppc64: all-skip
all-ppc: all-skip

all-i386: all-test
all-x86_64: all-test

all-test:
	${CC} ${LINKFLAGS} foo.c -dynamiclib -o libfoo.dylib -Wl,-w
	${DYLDINFO} -lazy_bind libfoo.dylib | grep _a | ${FAIL_IF_STDIN}
	${PASS_IFF_GOOD_MACHO} libfoo.dylib

all-skip:
	${PASS_IFF} true

clean:
	rm  -f  libfoo.dylib 

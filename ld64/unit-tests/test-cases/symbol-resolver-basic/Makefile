
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
	${CC} ${CCFLAGS} foo.c -c -o foo.o
	${CC} ${LINKFLAGS} foo.o -dynamiclib -o libfoo.dylib 
	${DYLDINFO} -export libfoo.dylib | grep _foo | grep resolver | ${FAIL_IF_EMPTY}
	${LD} -arch ${ARCH} -r foo.o -o foo-r.o
	${CC} ${LINKFLAGS} foo-r.o -dynamiclib -o libfoo-r.dylib 
	${DYLDINFO} -export libfoo-r.dylib | grep _foo | grep resolver | ${FAIL_IF_EMPTY}
	${PASS_IFF_GOOD_MACHO} libfoo-r.dylib

all-skip:
	${PASS_IFF} true

clean:
	rm  -f foo.o  libfoo.dylib foo-r.o libfoo-r.dylib

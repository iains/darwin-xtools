# Test that we add an OSO for each object file that is built from a source.

# The case is where a single source file might be compiled multiple times with
# conditional output - resulting in several object files related to the source.
# ld64 was only adding the OSO for the first object it saw for the source
# (leaving the remaining ones without an OSO).  This causes dsymutil to fail
# to find the relevant symbol file with the warning as below.

# warning: (....)  could not find object file symbol for symbol _bar

# Test and fix by Iain Sandoe <iain@sandoe.co.uk>

TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

all:
	${CC} ${CCFLAGS} foobar.c -c -o foo.o -g -DFOO
	${CC} ${CCFLAGS} foobar.c -c -o bar.o -g
	${CC} ${LINKFLAGS} fb-main.c -g foo.o bar.o -o fb 2>&1 | \
	grep "could not find object file symbol for symbol" | ${FAIL_IF_STDIN}
	${NM} -a fb | grep "OSO.*/missing-oso/bar.o" | ${FAIL_IF_EMPTY}
	${NM} -a fb | grep "OSO.*/missing-oso/foo.o" | ${FAIL_IF_EMPTY}
	${PASS_IFF_GOOD_MACHO} fb

clean:
	rm -f *.o fb


TESTROOT = ../..
include ${TESTROOT}/include/common.makefile


#
# Verify -ObjC works with x86_64 categories
# Verify stripped archives keep don't-strip bit
#

run: all

all:
	${CC} ${CCFLAGS} -g test.m -c -o test.o
	${CC} ${CCFLAGS} -g test2.m -c -o test2.o
	${LIBTOOL} -static test.o test2.o -o libtest.a
	${CC} ${CCFLAGS} main.m libtest.a -ObjC -o main -framework Foundation
	${NM} main | grep mycatmethod1 | ${FAIL_IF_EMPTY}
	${NM} main | grep mycatmethod2 | ${FAIL_IF_EMPTY}
	${NM} main | grep mymethod2 | ${FAIL_IF_EMPTY}
	${LD} -arch ${ARCH} -r -S -keep_private_externs test.o test2.o -o test-stripped.o
	${LIBTOOL} -static test-stripped.o -o libtest-stripped.a
	${CC} ${CCFLAGS} main.m libtest-stripped.a -all_load -dead_strip -o main2 -framework Foundation
	${NM} main2 | grep mycatmethod1 | ${FAIL_IF_EMPTY}
	${NM} main2 | grep mycatmethod2 | ${FAIL_IF_EMPTY}
	${NM} main2 | grep mymethod2 | ${FAIL_IF_EMPTY}
	${PASS_IFF_GOOD_MACHO} main2
	
clean:
	rm -rf test.o test2.o test-stripped.o libtest.a libtest-stripped.a main main2 

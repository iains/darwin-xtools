# Test that we adjust the page 0 size correctly when mdynamic-no-pic
# code is used with ppc64

TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

all: all-${ARCH}

all-i386: all-true
all-x86_64: all-true
all-ppc: all-true
all-arm: all-true

all-true:
	${PASS_IFF} true

# We should generate the 'normal' Darwin 4Gb page zero for PIC code, but since
# the constraints of mdynamic-no-pic mean the code must reside in the first
# 4G, override in this case and use the 4k page size used by ppc.  Pad the
# first 4G with an inaccessible filler.  So we should see a 4G page zero and
# no __4GBFILL segment for PIC code, and a 4k page zero and a __4GBFILL seg
# for mdynamic-no-pic.

all-ppc64:
	${CC} ${LINKFLAGS} main.c -fPIC -o main-p 
	${FAIL_IF_BAD_MACHO} main-p
	${OTOOL} -lv main-p | grep -A2 "__PAGEZERO" |grep "vmsize 0x0000000100000000" | ${FAIL_IF_EMPTY}
	${OTOOL} -lv main-p | grep "segname __4GBFILL" | ${FAIL_IF_STDIN}
	${CC} ${LINKFLAGS} main.c -mdynamic-no-pic -o main-dnp
	${FAIL_IF_BAD_MACHO} main-dnp
	${OTOOL} -lv main-dnp | grep -A2 "__PAGEZERO" |grep "vmsize 0x0000000000001000" | ${FAIL_IF_EMPTY}
	${OTOOL} -lv main-dnp | grep "segname __4GBFILL" | ${FAIL_IF_EMPTY}

clean:
	rm -f main-*

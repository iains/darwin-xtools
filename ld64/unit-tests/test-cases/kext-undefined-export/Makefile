
TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

#
# Sanity check that ld can link a kext
#

EXPORT_LIST = mykext.exp
FILE_TYPE = OBJECT
ifeq (${ARCH},x86_64)
	FILE_TYPE = KEXTBUNDLE
endif
ifeq (${FILEARCH},arm)
	FILE_TYPE = KEXTBUNDLE
endif
ifeq (${ARCH},i386)
	EXPORT_LIST = mykext-i386.exp
endif

all: all-${ARCH}

all-ppc: all-func
all-i386: all-func
all-x86_64: all-func
all-arm: all-func

# No kext for ppc64 so far.
all-ppc64: all-true

all-true:
	${PASS_IFF} true

all-func:
	${CC} ${CCFLAGS} -static -mkernel -c mykext.c -o mykext.o
	${CC} ${CCFLAGS} -static -mkernel -c mykextinfo.c -o mykextinfo.o
	${CC} ${LINKFLAGS} -Wl,-kext mykext.o mykextinfo.o -nostdlib -lkmodc++ -lkmod -lcc_kext -exported_symbols_list $(EXPORT_LIST) -o mykext -Wl,-w -Wl,-new_linker
	${OTOOL} -hv mykext | grep ${FILE_TYPE} | ${FAIL_IF_EMPTY}
	${NM} -nm mykext | grep '(undefined) external _extern_global' | ${FAIL_IF_EMPTY}
	${NM} -nm mykext | grep '(undefined) external _foo' | ${FAIL_IF_STDIN}
	${NM} -nm mykext | grep '(undefined) external _my_used_external_global' | ${FAIL_IF_EMPTY}
	${OTOOL} -rv mykext | grep '_extern_global' | ${FAIL_IF_EMPTY}
	${PASS_IFF} true

clean:
	rm -f mykext.o mykextinfo.o mykext

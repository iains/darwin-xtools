##
# Copyright (c) 2008 Apple Inc. All rights reserved.
#
# @APPLE_LICENSE_HEADER_START@
# 
# This file contains Original Code and/or Modifications of Original Code
# as defined in and that are subject to the Apple Public Source License
# Version 2.0 (the 'License'). You may not use this file except in
# compliance with the License. Please obtain a copy of the License at
# http://www.opensource.apple.com/apsl/ and read it before using this
# file.
# 
# The Original Code and all software distributed under the License are
# distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
# EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
# INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
# Please see the License for the specific language governing rights and
# limitations under the License.
# 
# @APPLE_LICENSE_HEADER_END@
##
TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

#
# Check the -force_load option.
# <rdar://problem/3738966> Need a linker option to load all objects from one library
#

run: all

all:
	${CC} ${CCFLAGS} foo.c -c -o foo-${ARCH}.o 
	${CC} ${CCFLAGS} bar.c -c -o bar-${ARCH}.o 
	${CC} ${CCFLAGS} baz.c -c -o baz-${ARCH}.o 
	${CC} ${CCFLAGS} bat.c -c -o bat-${ARCH}.o 
	${LIBTOOL} -static foo-${ARCH}.o bar-${ARCH}.o -o libfoobar-${ARCH}.a
	${LIBTOOL} -static baz-${ARCH}.o bat-${ARCH}.o -o libbazbat-${ARCH}.a
	${CC} ${LINKFLAGS} main.c -Wl,-force_load,libfoobar-${ARCH}.a libbazbat-${ARCH}.a -o main-${ARCH} -Wl,-why_load
	${NM} main-${ARCH} | grep "_bar" | ${FAIL_IF_EMPTY}
	${NM} main-${ARCH} | grep "_foo" | ${FAIL_IF_EMPTY}
	${NM} main-${ARCH} | grep "_baz" | ${FAIL_IF_STDIN}
	${NM} main-${ARCH} | grep "_bat" | ${FAIL_IF_STDIN}
	${PASS_IFF_GOOD_MACHO} main-${ARCH}

clean:
	rm -rf main-* *.o *.a

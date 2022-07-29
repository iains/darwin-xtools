##
# Copyright (c) 2011 Apple Inc. All rights reserved.
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
# Sanity check -dead_strip does not remove initializers and terminators
#


run: all

all:
	${CXX} ${LINKFLAGS} main.cxx other.cxx -dead_strip -o main-${ARCH}
	${FAIL_IF_BAD_MACHO} main-${ARCH}
	${NM} main-${ARCH} | grep dead_door_knob | ${FAIL_IF_STDIN}
	${NM} main-${ARCH} | grep ctr | ${FAIL_IF_EMPTY}
	${NM} main-${ARCH} | grep dtr | ${FAIL_IF_EMPTY}
	${CXX} ${LINKFLAGS} -fapple-kext -static main.cxx other.cxx -dead_strip -nostdlib -e _main -o main-static-${ARCH} -Wl,-new_linker
	${FAIL_IF_BAD_MACHO} main-static-${ARCH}
	${NM} main-static-${ARCH} | grep dead_door_knob | ${FAIL_IF_STDIN}
	${NM} main-static-${ARCH} | grep ctr | ${FAIL_IF_EMPTY}
	${NM} main-static-${ARCH} | grep dtr | ${FAIL_IF_EMPTY}
	${PASS_IFF_GOOD_MACHO} main-static-${ARCH}

clean:
	rm -rf main-${ARCH} main-static-${ARCH}

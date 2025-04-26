##
# Copyright (c) 2010 Apple Inc. All rights reserved.
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
# Test -bind-at-load option
#

run: all

all:
	${CC} ${LINKFLAGS} foo.c -dynamiclib -o libfoo-${ARCH}.dylib
	${CC} ${LINKFLAGS} bar.c -dynamiclib -o libbar-${ARCH}.dylib
	${CC} ${LINKFLAGS} main.c libfoo-${ARCH}.dylib libbar-${ARCH}.dylib -o main-${ARCH} -Wl,-bind_at_load
	${DYLDINFO} -lazy_bind main-${ARCH} | grep "no compressed lazy binding info" | ${FAIL_IF_EMPTY}
	${DYLDINFO} -bind main-${ARCH} | grep _bar | ${FAIL_IF_EMPTY}
	${PASS_IFF_GOOD_MACHO} main-${ARCH}
	
clean:
	rm -f *.dylib main-*

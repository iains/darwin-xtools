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
# Test that objc files loaded from archives set image info
#

IMAGE_INFO = "__image_info"

# OBJC2 archs.
ifeq ($(ARCH),ppc64)
	IMAGE_INFO = "__objc_imageinfo"
endif
ifeq ($(ARCH),x86_64)
	IMAGE_INFO = "__objc_imageinfo"
endif
ifeq (${FILEARCH},arm)
	IMAGE_INFO = "__objc_imageinfo"
endif

run: all

all:
	${CC} ${CCFLAGS} main.m -c -o main-${ARCH}.o
	${LIBTOOL} -static main-${ARCH}.o -o libmain-${ARCH}.a
	${CC} ${LINKFLAGS} libmain-${ARCH}.a -o main-${ARCH} -framework Foundation
	${SIZE} -l main-${ARCH} | grep ${IMAGE_INFO} | ${FAIL_IF_EMPTY}
	${PASS_IFF_GOOD_MACHO} main-${ARCH}

clean:
	rm -rf *.o *.a main-*

##
# Copyright (c) 2006-2007 Apple Inc. All rights reserved.
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

# Do these before including the common piece so that we can override MINOS.
DYLIB_EXTRA =

ifeq ($(ARCH),ppc)
  LD_VERS = -macosx_version_min 10.5
  DYLIB_EXTRA = -syslibroot ${SDK} -ldylib1.10.5.o
endif
ifeq ($(ARCH),ppc64)
  LD_VERS = -macosx_version_min 10.5
  DYLIB_EXTRA = -syslibroot ${SDK} -ldylib1.10.5.o
endif

# To use the ld64 internal dylib crts, we have to be on 10.6+ (or iOS4+)
ifeq ($(FILEARCH),arm)
  LD_VERS = -ios_version_min 4.0 -syslibroot $(IOS_SDK)
endif

ifeq ($(ARCH),i386)
  # To use the ld built-in version, we need to be on 10.6.
  LD_VERS = -macosx_version_min 10.6.8
  DYLIB_EXTRA = -syslibroot ${SDK}
  MINOS = 10.6.8
endif

ifeq ($(ARCH),x86_64)
  # To use the ld built-in version, we need to be on 10.6.
  LD_VERS = -macosx_version_min 10.6.8
  MINOS = 10.6.8
  DYLIB_EXTRA = -syslibroot ${SDK}
endif

include ${TESTROOT}/include/common.makefile

#
# Check that ld can figure out architecture from .o files without -arch option
#

run: all

all:
	${CC} ${CCFLAGS} hello.c -c -o hello-${ARCH}.o 
	${FAIL_IF_BAD_OBJ} hello-${ARCH}.o
	${LD} hello-${ARCH}.o -dylib ${DYLIB_EXTRA} -o hello-${ARCH}.dylib -lSystem $(LD_VERS) 
	file hello-${ARCH}.dylib | grep ${ARCH} | ${PASS_IFF_STDIN}

clean:
	rm -rf *.o hello-*.dylib

/*
 * Copyright (c) 2007 Apple Inc. All rights reserved.
 */
/*
 * FILE_ID: thread_status.h
 */


#ifndef _ARM_TGT_THREAD_STATUS_H_
#define _ARM_TGT_THREAD_STATUS_H_

#include <mach-for-target/arm/_structs.h>
#include <mach-for-target/arm/thread_state.h>

/*
 *    Support for determining the state of a thread
 */


/*
 *  Flavors
 */

#define ARM_THREAD_STATE		1
#define ARM_VFP_STATE			2
#define ARM_EXCEPTION_STATE		3
#define ARM_DEBUG_STATE			4 /* pre-armv8 */
#define THREAD_STATE_NONE		5
#define ARM_THREAD_STATE64		6
#define ARM_EXCEPTION_STATE64	7

/* ARM64_TODO: ref. ARM_SAVED_STATE64.  Separate these namespaces!  */

#ifdef XNU_KERNEL_PRIVATE
#define THREAD_STATE_LAST		8
#endif

/* For kernel use */
#define ARM_SAVED_STATE32		(THREAD_STATE_LAST+1)
#define ARM_SAVED_STATE64		(THREAD_STATE_LAST+2)
#define ARM_NEON_SAVED_STATE32		(THREAD_STATE_LAST+3)
#define ARM_NEON_SAVED_STATE64		(THREAD_STATE_LAST+4)
/* ARM_VFP_STATE64			(THREAD_STATE_LAST+5)  */
/* API */
#define ARM_DEBUG_STATE32		(THREAD_STATE_LAST+6)
#define ARM_DEBUG_STATE64		(THREAD_STATE_LAST+7)
#define ARM_NEON_STATE64		(THREAD_STATE_LAST+9)

#define VALID_THREAD_STATE_FLAVOR(x)\
((x == ARM_THREAD_STATE) 		||	\
 (x == ARM_VFP_STATE) 			||	\
 (x == ARM_EXCEPTION_STATE) 	||	\
 (x == ARM_DEBUG_STATE) 		||	\
 (x == THREAD_STATE_NONE)		||  \
 (x == ARM_NEON_STATE)		||	\
 (x == ARM_DEBUG_STATE32)	||	\
 (x == ARM_THREAD_STATE64)		||	\
 (x == ARM_EXCEPTION_STATE64)	||	\
 (x == ARM_NEON_STATE64)		||	\
 (x == ARM_DEBUG_STATE64))

typedef _STRUCT_ARM_TGT_THREAD_STATE		arm_tgt_thread_state_t;
typedef _STRUCT_ARM_TGT_THREAD_STATE64		arm_tgt_thread_state64_t;
typedef _STRUCT_ARM_TGT_VFP_STATE		arm_tgt_vfp_state_t;
typedef _STRUCT_ARM_TGT_NEON_STATE		arm_tgt_neon_state_t;
typedef _STRUCT_ARM_TGT_NEON_STATE64		arm_tgt_neon_state64_t;
typedef _STRUCT_ARM_TGT_EXCEPTION_STATE		arm_tgt_exception_state_t;
typedef _STRUCT_ARM_TGT_EXCEPTION_STATE64	arm_tgt_exception_state64_t;

/* Not ARM64 kernel and we must preserve legacy ARM definitions of
 * arm_debug_state for binary compatability of userland consumers of this file.
 */
typedef _STRUCT_ARM_TGT_DEBUG_STATE		arm_tgt_debug_state_t;
typedef _STRUCT_ARM_TGT_LEGACY_DEBUG_STATE	arm_tgt_legacy_debug_state_t;
typedef _STRUCT_ARM_TGT_DEBUG_STATE32		arm_tgt_debug_state32_t;
typedef _STRUCT_ARM_TGT_DEBUG_STATE64		arm_tgt_debug_state64_t;

#define ARM_TGT_THREAD_STATE_COUNT ((mach_msg_type_number_t) \
   (sizeof (arm_tgt_thread_state_t)/sizeof(uint32_t)))

#define ARM_TGT_THREAD_STATE64_COUNT ((mach_msg_type_number_t) \
   (sizeof (arm_tgt_thread_state64_t)/sizeof(uint32_t)))

#define ARM_TGT_VFP_STATE_COUNT ((mach_msg_type_number_t) \
   (sizeof (arm_tgt_vfp_state_t)/sizeof(uint32_t)))

#define ARM_TGT_EXCEPTION_STATE_COUNT ((mach_msg_type_number_t) \
   (sizeof (arm_tgt_exception_state_t)/sizeof(uint32_t)))

#define ARM_TGT_EXCEPTION_STATE64_COUNT ((mach_msg_type_number_t) \
   (sizeof (arm_tgt_exception_state64_t)/sizeof(uint32_t)))

#define ARM_TGT_DEBUG_STATE_COUNT ((mach_msg_type_number_t) \
   (sizeof (arm_tgt_debug_state_t)/sizeof(uint32_t)))

#define ARM_TGT_DEBUG_STATE32_COUNT ((mach_msg_type_number_t) \
   (sizeof (arm_tgt_debug_state32_t)/sizeof(uint32_t)))

#define ARM_TGT_DEBUG_STATE64_COUNT ((mach_msg_type_number_t) \
   (sizeof (arm_tgt_debug_state64_t)/sizeof(uint32_t)))

#define MACHINE_THREAD_STATE ARM_THREAD_STATE64
#define MACHINE_THREAD_STATE_COUNT  ARM_TGT_THREAD_STATE_COUNT64

/*
 * Largest state on this machine:
 */
#define THREAD_MACHINE_STATE_MAX	THREAD_STATE_MAX

#endif    /* _ARM_TGT_THREAD_STATUS_H_ */

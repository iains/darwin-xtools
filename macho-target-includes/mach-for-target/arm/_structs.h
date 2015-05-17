/*
 * Copyright (c) 2004-2007 Apple Inc. All rights reserved.
 */
/*
 * @OSF_COPYRIGHT@
 */
#ifndef	_MACH_FOR_TGT_ARM__STRUCTS_H_
#define	_MACH_FOR_TGT_ARM__STRUCTS_H_

#define _STRUCT_ARM_TGT_EXCEPTION_STATE	struct __darwin_arm_tgt_exception_state
_STRUCT_ARM_TGT_EXCEPTION_STATE
{
  uint32_t	__exception; /* number of arm exception taken */
  uint32_t	__fsr; /* Fault status */
 uint32_t	__far; /* Virtual Fault Address */
};

#define _STRUCT_ARM_TGT_EXCEPTION_STATE64	struct __darwin_arm_tgt_exception_state64
_STRUCT_ARM_TGT_EXCEPTION_STATE64
{
  uint64_t	__far; /* Virtual Fault Address */
  uint32_t	__esr; /* Exception syndrome */
  uint32_t	__exception; /* number of arm exception taken */
};

#define _STRUCT_ARM_TGT_THREAD_STATE	struct __darwin_arm_tgt_thread_state
_STRUCT_ARM_TGT_THREAD_STATE
{
  uint32_t	__r[13];	/* General purpose register r0-r12 */
  uint32_t	__sp;		/* Stack pointer r13 */
  uint32_t	__lr;		/* Link register r14 */
  uint32_t	__pc;		/* Program counter r15 */
  uint32_t	__cpsr;		/* Current program status register */
};

#define _STRUCT_ARM_TGT_THREAD_STATE64	struct __darwin_arm_tgt_thread_state64
_STRUCT_ARM_TGT_THREAD_STATE64
{
  uint64_t    __x[29];	/* General purpose registers x0-x28 */
  uint64_t    __fp;		/* Frame pointer x29 */
  uint64_t    __lr;		/* Link register x30 */
  uint64_t    __sp;		/* Stack pointer x31 */
  uint64_t    __pc;		/* Program counter */
  uint32_t    __cpsr;	/* Current program status register */
};

#define _STRUCT_ARM_TGT_VFP_STATE		struct __darwin_arm_tgt_vfp_state
_STRUCT_ARM_TGT_VFP_STATE
{
  uint32_t        __r[64];
  uint32_t        __fpscr;
};

#define _STRUCT_ARM_TGT_NEON_STATE64		struct __darwin_arm_tgt_neon_state64
#define _STRUCT_ARM_TGT_NEON_STATE		struct __darwin_arm_tgt_neon_state

/*
 * No 128-bit intrinsic for ARM; leave it opaque for now.
 */
_STRUCT_ARM_TGT_NEON_STATE64 
{
  uint64_t __v[32*2];
  uint32_t __fpsr;
  uint32_t __fpcr;
} __attribute__((__aligned__(16)));

_STRUCT_ARM_TGT_NEON_STATE
{
  uint64_t __v[16*2];
  uint32_t __fpsr;
  uint32_t __fpcr;
} __attribute__((__aligned__(16)));

/*
 * Debug State
 */

#define _STRUCT_ARM_TGT_DEBUG_STATE	struct __darwin_arm_tgt_debug_state
_STRUCT_ARM_TGT_DEBUG_STATE
{
  uint32_t        __bvr[16];
  uint32_t        __bcr[16];
  uint32_t        __wvr[16];
  uint32_t        __wcr[16];
};

#define _STRUCT_ARM_TGT_LEGACY_DEBUG_STATE	struct arm_tgt_legacy_debug_state
_STRUCT_ARM_TGT_LEGACY_DEBUG_STATE
{
  uint32_t        __bvr[16];
  uint32_t        __bcr[16];
  uint32_t        __wvr[16];
  uint32_t        __wcr[16];
};

#define _STRUCT_ARM_TGT_DEBUG_STATE32	struct __darwin_arm_tgt_debug_state32
_STRUCT_ARM_TGT_DEBUG_STATE32
{
  uint32_t        __bvr[16];
  uint32_t        __bcr[16];
  uint32_t        __wvr[16];
  uint32_t        __wcr[16];
  uint64_t	  __mdscr_el1; /* Bit 0 is SS (Hardware Single Step) */
};

#define _STRUCT_ARM_TGT_DEBUG_STATE64	struct __darwin_arm_tgt_debug_state64
_STRUCT_ARM_TGT_DEBUG_STATE64
{
  uint64_t        __bvr[16];
  uint64_t        __bcr[16];
  uint64_t        __wvr[16];
  uint64_t        __wcr[16];
  uint64_t	  __mdscr_el1; /* Bit 0 is SS (Hardware Single Step) */
};

#endif /* _MACH_FOR_TGT_ARM__STRUCTS_H_ */

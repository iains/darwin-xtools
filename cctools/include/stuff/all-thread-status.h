#ifndef __ALL_THREAD_STATUS_H
#define __ALL_THREAD_STATUS_H

#undef _POSIX_C_SOURCE
#undef _DARWIN_C_SOURCE
typedef uint32_t mach_msg_type_number_t;

#include <mach/m68k/thread_status.h>
#undef MACHINE_THREAD_STATE	/* need to undef these to avoid warnings */
#undef MACHINE_THREAD_STATE_COUNT
#undef THREAD_STATE_NONE
#undef VALID_THREAD_STATE_FLAVOR
#include <mach-for-target/ppc/thread_status.h>
#undef MACHINE_THREAD_STATE	/* need to undef these to avoid warnings */
#undef MACHINE_THREAD_STATE_COUNT
#undef THREAD_STATE_NONE
#undef VALID_THREAD_STATE_FLAVOR
#include <mach/m88k/thread_status.h>
#include <mach/i860/thread_status.h>
#include <mach-for-target/i386/thread_status.h>
#include <mach/hppa/thread_status.h>
#include <mach/sparc/thread_status.h>
#include <mach-for-target/arm/thread_status.h>

#endif /* __ALL_THREAD_STATUS_H */
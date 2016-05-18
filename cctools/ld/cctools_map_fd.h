/* Iain Sandoe, Oct 2015. */

#ifndef __CCTOOLS_MAP_FD_H_
#define __CCTOOLS_MAP_FD_H_

#include <mach/kern_return.h>
#include <mach/vm_types.h>

/* The interface is originally defined in libc.h.   */

kern_return_t map_fd(int fd, vm_offset_t offset,
                    vm_offset_t *addr, boolean_t find_space,
                    vm_size_t numbytes);

#endif /* __CCTOOLS_MAP_FD_H_ */

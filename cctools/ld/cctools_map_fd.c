/* Iain Sandoe, Oct 2015. */

#include "cctools_map_fd.h"
#include <sys/mman.h>

/* The interface is originally defined in libc.h   */

/* The actual code for the map_fd syscall is in kern_mman.c in xnu.
   - replace it with a call to mmap.  */

kern_return_t 
map_fd(int fd, vm_offset_t offset, vm_offset_t *addr,
       boolean_t find_space, vm_size_t numbytes)
{
  void *ret_addr = (void *)0; /* default, let the system pick the adddress.  */
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_FILE | MAP_PRIVATE;

  if (! find_space) {
    /* Use the address specified.
       At present, this is not used by cctools - and presumably the code will
       be updated on some future drop to remove it.  */
    ret_addr = (void *) *addr;
    flags |= MAP_FIXED;
  }
     
  ret_addr = mmap(ret_addr, numbytes, prot, flags, fd, offset);

  /* mmp either returns MAP_FAILED, or the result.  */
  if (ret_addr == MAP_FAILED)
    return KERN_FAILURE;

  *addr = (vm_offset_t)ret_addr;
  return KERN_SUCCESS;
}

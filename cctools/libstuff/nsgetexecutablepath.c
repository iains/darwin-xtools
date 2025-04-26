
#include <unistd.h>

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#include <sys/sysctl.h>
#endif

#include "stuff/nsgetexecutablepath.h"

int
_NSGetExecutablePath(char *path, unsigned int *size)
{
#ifdef __FreeBSD__
   int mib[4];
   mib[0] = CTL_KERN;
   mib[1] = KERN_PROC;
   mib[2] = KERN_PROC_PATHNAME;
   mib[3] = -1;
   size_t cb = *size;
   if (sysctl(mib, 4, path, &cb, NULL, 0) != 0)
    return -1;
   *size = cb;
   return 0;
#else
   int bufsize = *size;
   int ret_size;
   ret_size = readlink("/proc/self/exe", path, bufsize-1);
   if (ret_size != -1)
   {
        *size = ret_size;
        path[ret_size]=0;
        return 0;
   }
   else
    return -1;
#endif
}

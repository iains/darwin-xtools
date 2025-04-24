#if !defined (MAXPATHLEN)
# if defined (PATH_MAX)
#  define MAXPATHLEN PATH_MAX
# else
#  define MAXPATHLEN 1024
# endif
#endif

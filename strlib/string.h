#ifndef _XTOOLS_STRING_H_
#define _XTOOLS_STRING_H_

/* Addtional string functions that are not available everywhere, we provide
   fall-backs (although they may also be provided as built-in).  */

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS
size_t strnlen(const char *s, size_t maxlen);
char * strndup(const char *s1, size_t n);
__END_DECLS

#include_next <string.h>

#endif /* _XTOOLS_STRING_H_ */

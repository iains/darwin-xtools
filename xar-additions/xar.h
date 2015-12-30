#ifndef _XTOOLS_XARLIB_H_
#define _XTOOLS_XARLIB_H_

/* Addtional string functions that are not available everywhere, we provide
   fall-backs (although they may also be provided as built-in).  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include_next <xar/xar.h>

__BEGIN_DECLS
int32_t xar_extract_tobuffersz(xar_t x, xar_file_t f, char **buffer, size_t *size);

__END_DECLS

#endif /* _XTOOLS_XARLIB_H_ */

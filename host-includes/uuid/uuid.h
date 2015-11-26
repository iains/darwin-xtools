#ifndef _XTOOLS_UUID_UUID_H
#define _XTOOLS_UUID_UUID_H

#include_next <uuid/uuid.h>

#ifndef _UUID_STRING_T
#define _UUID_STRING_T
typedef char    __darwin_uuid_string_t[37];
typedef __darwin_uuid_string_t  uuid_string_t;
#endif /* _UUID_STRING_T */


#endif /* _XTOOLS_UUID_UUID_H */

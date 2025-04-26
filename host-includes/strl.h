
#if !defined (__APPLE__)

#include <stddef.h>
#include <string.h>

inline size_t
strlcpy(char * __restrict dst, const char * __restrict src, size_t maxlen) {
    const size_t srclen = strlen(src);
    if (srclen < maxlen) {
        memcpy(dst, src, srclen+1);
    } else if (maxlen != 0) {
        memcpy(dst, src, maxlen-1);
        dst[maxlen-1] = '\0';
    }
    return srclen;
}

inline size_t
strlcat(char * __restrict dst, const char * __restrict src, size_t maxlen) {
    const size_t srclen = strlen(src);
    const size_t dstlen = strnlen(dst, maxlen);
    if (dstlen == maxlen) return maxlen+srclen;
    if (srclen < maxlen-dstlen) {
        memcpy(dst+dstlen, src, srclen+1);
    } else {
        memcpy(dst+dstlen, src, maxlen-dstlen-1);
        dst[maxlen-1] = '\0';
    }
    return dstlen + srclen;
}

#endif // Apple.
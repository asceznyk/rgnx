#ifndef RGX_STRING_H
#define RGX_STRING_H

#include <string.h>

#include <rgx_core.h>

#define rgx_memzero(buf, n) (void) memset(buf, 0, n)
#define rgx_memcpy(dst, src, n) (void) memcpy(dst, src, n)
#define rgx_cpymem(dst, src, n) (((u_char *)memcpy(dst, src, n)) + (n))

#endif

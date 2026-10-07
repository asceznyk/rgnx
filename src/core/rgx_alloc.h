#ifndef RGX_ALLOC_H
#define RGX_ALLOC_H

#include <stdlib.h>

#include <rgx_core.h>

void *rgx_malloc(size_t size);
void *rgx_calloc(size_t size);
void *rgx_memalign(size_t align, size_t size);
void rgx_free(void *p);

#endif

#ifndef RGX_ARRAY_H
#define RGX_ARRAY_H

typedef struct rgx_array_s {
  void *elts;
  rgx_uint_t nelts;
  size_t size;
  rgx_uint_t nalloc;
  rgx_arena_t *arena;
} rgx_array_t;

#endif


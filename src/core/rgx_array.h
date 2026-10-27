#ifndef RGX_ARRAY_H
#define RGX_ARRAY_H

typedef struct rgx_array_s {
  void *elts;
  rgx_uint_t nelts;
  size_t esize;
  rgx_uint_t nalloc;
  rgx_arena_t *arena;
} rgx_array_t;

rgx_array_t *rgx_array_create(rgx_arena_t *arena, rgx_uint_t n, size_t size);
void *rgx_array_push_n(rgx_array_t *array, rgx_uint_t n);
void *rgx_array_push(rgx_array_t *array);

static rgx_int_t
rgx_array_init(
  rgx_array_t *array, rgx_arena_t *arena, rgx_uint_t n, size_t size
)
{
  array->nelts = 0;
  array->esize = size;
  array->nalloc = n;
  array->arena = arena;
  array->elts = (void *)rgx_arena_alloc(arena, n*size);
  if (array->elts == NULL) {
    rgx_log_error("rgx_array_init: rgx_arena_alloc FAILED!");
    return RGX_ERROR;
  }
  return RGX_OK;
}

#endif


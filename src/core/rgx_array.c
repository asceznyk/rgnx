#include <rgx_core.h>

rgx_array_t*
rgx_array_create(rgx_arena_t *arena, rgx_uint_t n, size_t size)
{
  rgx_array_t *array = (void *)rgx_arena_alloc(arena, sizeof(rgx_array_t));
  if (array == NULL) {
    rgx_log_error("rgx_array_create: rgx_arena_alloc FAILED!");
    return NULL;
  }
  if (rgx_array_init(array, arena, n, size) == RGX_ERROR) {
    rgx_log_error("rgx_array_create: rgx_array_init FAILED!");
    return NULL;
  }
  return array;
}

void *
rgx_array_push_n(rgx_array_t *array, rgx_uint_t n)
{
  void *elt, *new;
  rgx_uint_t nalloc;
  size_t asize;
  rgx_arena_t *arena = array->arena;
  if (array->nelts + n > array->nalloc) {
    asize = n * array->esize;
    rgx_chunk_t *active = arena->active;
    if (asize > active->size - active->used) {
      nalloc = 2 * ((n >= array->nalloc) ? n : array->nalloc);
      new = rgx_arena_alloc(arena, nalloc*array->esize);
      if (new == NULL) {
        rgx_log_error("rgx_array_push_n: rgx_arena_alloc FAILED!");
        return NULL;
      }
      rgx_memcpy(new, array->elts, array->esize * array->nalloc);
      array->elts = new;
      array->nalloc = nalloc;
    } else {
      active->used += asize;
      array->nalloc += n;
    }
  }
  elt = (u_char *)array->elts + (array->nelts * array->esize);
  array->nelts += n;
  return elt;
}

void *
rgx_array_push(rgx_array_t *array)
{
  return rgx_array_push_n(array, 1);
}



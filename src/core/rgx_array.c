#include <rgx_core.h>

rgx_array_t*
rgx_array_create(rgx_arena_t *arena, rgx_uint_t n, size_t size)
{
  rgx_array_t *array = (void *)rgx_arena_alloc(arena, sizeof(rgx_array_t));
  rgx_log_info(
    "rgx_array_create: arena->active->used = %zu",
    arena->active->used
  );
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

void
rgx_array_destroy(rgx_array_t *array)
{
  array->elts = NULL;
  array = NULL;
}





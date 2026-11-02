#include <rgx_core.h>

rgx_list_t *
rgx_list_create(rgx_arena_t *arena, rgx_uint_t n, size_t size)
{
  rgx_list_t *list = (void *)rgx_arena_alloc(arena, sizeof(rgx_list_t));
  if (list == NULL) {
    rgx_log_error("rgx_list_create: rgx_arena_alloc FAILED!");
    return NULL;
  }
  if (rgx_list_init(list, arena, n, size) != RGX_OK) {
    rgx_log_error("rgx_list_create: rgx_array_init FAILED!");
    return NULL;
  }
  return list;
}

void *
rgx_list_push(rgx_list_t *list)
{
  void *elt;
  rgx_list_part_t *last;
  rgx_arena_t *arena = list->arena;
  last = list->last;
  if (last->nelts == list->nalloc) {
    last = (rgx_list_part_t *)rgx_arena_alloc(arena, sizeof(rgx_list_part_t));
    if (last == NULL)
      return NULL;
    last->elts = (void *)rgx_arena_alloc(arena, list->size * list->nalloc);
    if (last->elts == NULL)
      return NULL;
    last->next = NULL;
    last->nelts = 0;
    list->last->next = last;
    list->last = last;
  }
  elt = (u_char *)last->elts + (list->size * last->nelts);
  last->nelts++;
  return elt;
}




#ifndef RGX_LIST_H
#define RGX_LIST_H

#include <rgx_core.h>

typedef struct rgx_list_part_s rgx_list_part_t;

struct rgx_list_part_s {
  void *elts;
  rgx_uint_t nelts;
  rgx_list_part_t *next;
};

typedef struct {
  rgx_list_part_t *last;
  rgx_list_part_t part;
  size_t size;
  rgx_uint_t nalloc;
  rgx_arena_t *arena;
} rgx_list_t;

static rgx_int_t
rgx_list_init(rgx_list_t *list, rgx_arena_t *arena, rgx_uint_t n, size_t size)
{
  list->part.elts = rgx_arena_alloc(arena, n*size);
  if (list->part.elts == NULL)
    return RGX_ERROR;
  list->part.nelts = 0;
  list->part.next = NULL;
  list->last = &list->part;
  list->size = size;
  list->nalloc = n;
  list->arena = arena;
  return RGX_OK;
}

rgx_list_t *rgx_list_create(rgx_arena_t *arena, rgx_uint_t n, size_t size);
void *rgx_list_push(rgx_list_t *list);

#endif


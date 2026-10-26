#include <assert.h>
#include <stdio.h>

#include <rgx_core.h>

static void
test_array_create(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_array_t *array = rgx_array_create(arena, 4, sizeof(rgx_uint_t));
  assert(array != NULL);
  assert(array->elts != NULL);
  assert(array->nelts == 0);
  assert(array->nalloc == 4);
  assert(array->size == sizeof(rgx_uint_t));
  assert(array->arena == arena);
  rgx_arena_destroy(arena);
  puts("test_array_create: SUCCESS");
}

static void
test_array_destroy_clears_elts(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_array_t *array = rgx_array_create(arena, 7, sizeof(rgx_uint_t));
  assert(array != NULL);
  assert(array->elts != NULL);
  size_t used_before = arena->active->used;
  rgx_array_destroy(array);
  assert(array != NULL);
  assert(array->elts == NULL);
  assert(arena->active->used == used_before);
  assert(array->arena == arena);
  rgx_arena_destroy(arena);
  puts("test_array_destroy_clears_elts: SUCCESS");
}

static void
test_array_destroy_lifo(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_array_t *a = rgx_array_create(arena, 3, sizeof(rgx_uint_t));
  assert(a != NULL);
  rgx_array_t *b = rgx_array_create(arena, 2, sizeof(rgx_uint_t));
  assert(b != NULL);
  rgx_array_t *c = rgx_array_create(arena, 6, sizeof(rgx_uint_t));
  assert(c != NULL);
  size_t used_before = arena->active->used;
  rgx_array_destroy(c);
  assert(c->elts == NULL);
  assert(arena->active->used == used_before);
  rgx_array_destroy(b);
  assert(b->elts == NULL);
  assert(arena->active->used == used_before);
  rgx_array_destroy(a);
  assert(a->elts == NULL);
  assert(arena->active->used == used_before);
  rgx_arena_destroy(arena);
  puts("test_array_destroy_lifo: SUCCESS");
}

static void
test_array_destroy_preserves_later_allocation(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_array_t *array = rgx_array_create(arena, 4, sizeof(rgx_uint_t));
  assert(array != NULL);
  u_char *later = rgx_arena_alloc(arena, 32);
  assert(later != NULL);
  size_t used_before = arena->active->used;
  rgx_array_destroy(array);
  assert(array->elts == NULL);
  assert(arena->active->used == used_before);
  assert(later != NULL);
  rgx_arena_destroy(arena);
  puts("test_array_destroy_preserves_later_allocation: SUCCESS");
}

static void
test_array_destroy_preserves_other_allocations(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  u_char *before = rgx_arena_alloc(arena, 32);
  assert(before != NULL);
  rgx_array_t *array = rgx_array_create(arena, 4, sizeof(rgx_uint_t));
  assert(array != NULL);
  u_char *after = rgx_arena_alloc(arena, 32);
  assert(after != NULL);
  size_t used_before = arena->active->used;
  rgx_array_destroy(array);
  assert(array->elts == NULL);
  assert(arena->active->used == used_before);
  assert(before != after);
  rgx_arena_destroy(arena);
  puts("test_array_destroy_preserves_other_allocations: SUCCESS");
}

int
main(void)
{
  test_array_create();
  test_array_destroy_clears_elts();
  test_array_destroy_lifo();
  test_array_destroy_preserves_later_allocation();
  test_array_destroy_preserves_other_allocations();
  puts("main: All array tests passed!");
  return 0;
}


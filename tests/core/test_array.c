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
  assert(array->esize == sizeof(rgx_uint_t));
  assert(array->arena == arena);
  rgx_arena_destroy(arena);
  puts("test_array_create: SUCCESS");
}

static void
test_array_push_single(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_array_t *array = rgx_array_create(arena, 4, sizeof(rgx_uint_t));
  assert(array != NULL);
  rgx_uint_t *elt = rgx_array_push(array);
  assert(elt != NULL);
  *elt = 42;
  assert(array->nelts == 1);
  assert(array->nalloc == 4);
  assert(((rgx_uint_t *) array->elts)[0] == 42);
  rgx_arena_destroy(arena);
  puts("test_array_push_single: SUCCESS");
}

static void
test_array_push_to_capacity(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_array_t *array = rgx_array_create(arena, 4, sizeof(rgx_uint_t));
  assert(array != NULL);
  for (rgx_uint_t i = 0; i < 4; i++) {
    rgx_uint_t *elt = rgx_array_push(array);
    assert(elt != NULL);
    *elt = i + 1;
  }
  assert(array->nelts == 4);
  assert(array->nalloc == 4);
  for (rgx_uint_t i = 0; i < 4; i++) {
    assert(((rgx_uint_t *) array->elts)[i] == i + 1);
  }
  rgx_arena_destroy(arena);
  puts("test_array_push_to_capacity: SUCCESS");
}

static void
test_array_push_grow(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_array_t *array = rgx_array_create(arena, 2, sizeof(rgx_uint_t));
  assert(array != NULL);
  for (rgx_uint_t i = 0; i < 2; i++) {
    rgx_uint_t *elt = rgx_array_push(array);
    assert(elt != NULL);
    *elt = (i + 1) * 10;
  }
  assert(array->nelts == 2);
  rgx_uint_t *elt = rgx_array_push(array);
  assert(elt != NULL);
  *elt = 30;
  assert(array->nelts == 3);
  assert(array->nalloc >= 3);
  rgx_uint_t *elts = array->elts;
  assert(elts[0] == 10);
  assert(elts[1] == 20);
  assert(elts[2] == 30);
  rgx_arena_destroy(arena);
  puts("test_array_push_grow: SUCCESS");
}

static void
test_array_push_multiple_growths(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_array_t *array = rgx_array_create(arena, 2, sizeof(rgx_uint_t));
  assert(array != NULL);
  for (rgx_uint_t i = 0; i < 16; i++) {
    rgx_uint_t *elt = rgx_array_push(array);
    assert(elt != NULL);
    *elt = i * 3;
  }
  assert(array->nelts == 16);
  assert(array->nalloc >= array->nelts);
  rgx_uint_t *elts = array->elts;
  for (rgx_uint_t i = 0; i < 16; i++) {
    assert(elts[i] == i * 3);
  }
  rgx_arena_destroy(arena);
  puts("test_array_push_multiple_growths: SUCCESS");
}

static void
test_array_push_n_batch(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_array_t *array = rgx_array_create(arena, 8, sizeof(rgx_uint_t));
  assert(array != NULL);
  rgx_uint_t *elts = rgx_array_push_n(array, 3);
  assert(elts != NULL);
  elts[0] = 11;
  elts[1] = 22;
  elts[2] = 33;
  assert(array->nelts == 3);
  assert(array->nalloc == 8);
  assert(((rgx_uint_t *) array->elts)[0] == 11);
  assert(((rgx_uint_t *) array->elts)[1] == 22);
  assert(((rgx_uint_t *) array->elts)[2] == 33);
  rgx_arena_destroy(arena);
  puts("test_array_push_n_batch: SUCCESS");
}

static void
test_array_push_n_after_existing_elements(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_array_t *array = rgx_array_create(arena, 8, sizeof(rgx_uint_t));
  assert(array != NULL);
  rgx_uint_t *first = rgx_array_push(array);
  assert(first != NULL);
  *first = 100;
  rgx_uint_t *batch = rgx_array_push_n(array, 3);
  assert(batch != NULL);
  batch[0] = 200;
  batch[1] = 300;
  batch[2] = 400;
  assert(array->nelts == 4);
  rgx_uint_t *elts = array->elts;
  assert(elts[0] == 100);
  assert(elts[1] == 200);
  assert(elts[2] == 300);
  assert(elts[3] == 400);
  rgx_arena_destroy(arena);
  puts("test_array_push_n_after_existing_elements: SUCCESS");
}

static void
test_array_push_n_grow(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_array_t *array = rgx_array_create(arena, 2, sizeof(rgx_uint_t));
  assert(array != NULL);
  rgx_uint_t *first = rgx_array_push_n(array, 2);
  assert(first != NULL);
  first[0] = 7;
  first[1] = 14;
  rgx_uint_t *batch = rgx_array_push_n(array, 3);
  assert(batch != NULL);
  batch[0] = 21;
  batch[1] = 28;
  batch[2] = 35;
  assert(array->nelts == 5);
  assert(array->nalloc >= 5);
  rgx_uint_t *elts = array->elts;
  for (rgx_uint_t i = 0; i < 5; i++) {
    assert(elts[i] == (i + 1) * 7);
  }
  rgx_arena_destroy(arena);
  puts("test_array_push_n_grow: SUCCESS");
}

static void
test_array_push_n_larger_than_initial_capacity(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_array_t *array = rgx_array_create(arena, 2, sizeof(rgx_uint_t));
  assert(array != NULL);
  rgx_uint_t *batch = rgx_array_push_n(array, 6);
  assert(batch != NULL);
  assert(array->nelts == 6);
  assert(array->nalloc >= 6);
  for (rgx_uint_t i = 0; i < 6; i++) {
    batch[i] = i + 10;
  }
  rgx_uint_t *elts = array->elts;
  for (rgx_uint_t i = 0; i < 6; i++) {
    assert(elts[i] == i + 10);
  }
  rgx_arena_destroy(arena);
  puts("test_array_push_n_larger_than_initial_capacity: SUCCESS");
}

int
main(void)
{
  test_array_create();
  test_array_push_single();
  test_array_push_to_capacity();
  test_array_push_grow();
  test_array_push_multiple_growths();
  test_array_push_n_batch();
  test_array_push_n_after_existing_elements();
  test_array_push_n_grow();
  test_array_push_n_larger_than_initial_capacity();
  puts("main: All array tests passed!");
  return 0;
}


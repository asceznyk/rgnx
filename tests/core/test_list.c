#include <assert.h>
#include <stdio.h>

#include <rgx_core.h>
#include <rgx_list.h>

static void
test_list_create(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_list_t *list = rgx_list_create(arena, 4, sizeof(rgx_uint_t));
  assert(list != NULL);
  assert(list->part.elts != NULL);
  assert(list->part.nelts == 0);
  assert(list->part.next == NULL);
  assert(list->last == &list->part);
  assert(list->size == sizeof(rgx_uint_t));
  assert(list->nalloc == 4);
  assert(list->arena == arena);
  rgx_arena_destroy(arena);
  puts("test_list_create: SUCCESS");
}

static void
test_list_push_single(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_list_t *list = rgx_list_create(arena, 4, sizeof(rgx_uint_t));
  assert(list != NULL);
  rgx_uint_t *elt = rgx_list_push(list);
  assert(elt != NULL);
  *elt = 42;
  assert(list->part.nelts == 1);
  assert(list->last == &list->part);
  assert(((rgx_uint_t *)list->part.elts)[0] == 42);
  rgx_arena_destroy(arena);
  puts("test_list_push_single: SUCCESS");
}

static void
test_list_push_to_capacity(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_list_t *list = rgx_list_create(arena, 4, sizeof(rgx_uint_t));
  assert(list != NULL);
  for (rgx_uint_t i = 0; i < 4; i++) {
    rgx_uint_t *elt = rgx_list_push(list);
    assert(elt != NULL);
    *elt = i + 1;
  }
  assert(list->part.nelts == 4);
  assert(list->part.next == NULL);
  assert(list->last == &list->part);
  for (rgx_uint_t i = 0; i < 4; i++) {
    assert(((rgx_uint_t *)list->part.elts)[i] == i + 1);
  }
  rgx_arena_destroy(arena);
  puts("test_list_push_to_capacity: SUCCESS");
}

static void
test_list_push_grow(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_list_t *list = rgx_list_create(arena, 2, sizeof(rgx_uint_t));
  assert(list != NULL);
  rgx_uint_t *first = rgx_list_push(list);
  assert(first != NULL);
  *first = 10;
  rgx_uint_t *second = rgx_list_push(list);
  assert(second != NULL);
  *second = 20;
  assert(list->part.nelts == 2);
  rgx_uint_t *third = rgx_list_push(list);
  assert(third != NULL);
  *third = 30;
  assert(list->part.nelts == 2);
  assert(list->part.next != NULL);
  assert(list->last == list->part.next);
  assert(list->last->nelts == 1);
  assert(((rgx_uint_t *)list->part.elts)[0] == 10);
  assert(((rgx_uint_t *)list->part.elts)[1] == 20);
  assert(((rgx_uint_t *)list->last->elts)[0] == 30);
  rgx_arena_destroy(arena);
  puts("test_list_push_grow: SUCCESS");
}

static void
test_list_push_multiple_parts(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_list_t *list = rgx_list_create(arena, 2, sizeof(rgx_uint_t));
  assert(list != NULL);
  for (rgx_uint_t i = 0; i < 10; i++) {
    rgx_uint_t *elt = rgx_list_push(list);
    assert(elt != NULL);
    *elt = i * 3;
  }
  assert(list->part.nelts == 2);
  assert(list->part.next != NULL);
  assert(list->part.next->next != NULL);
  assert(list->part.next->next->next != NULL);
  assert(list->part.next->next->next->next != NULL);
  assert(list->part.next->next->next->next->next == NULL);
  assert(list->last == list->part.next->next->next->next);
  rgx_list_part_t *part = &list->part;
  rgx_uint_t i = 0;
  while (part != NULL) {
    rgx_uint_t *elts = part->elts;
    for (rgx_uint_t j = 0; j < part->nelts; j++) {
      assert(elts[j] == i * 3);
      i++;
    }
    part = part->next;
  }
  assert(i == 10);
  rgx_arena_destroy(arena);
  puts("test_list_push_multiple_parts: SUCCESS");
}

static void
test_list_push_struct_elements(void)
{
  typedef struct {
    rgx_uint_t id;
    rgx_uint_t value;
  } test_element_t;
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  rgx_list_t *list = rgx_list_create(arena, 2, sizeof(test_element_t));
  assert(list != NULL);
  for (rgx_uint_t i = 0; i < 3; i++) {
    test_element_t *elt = rgx_list_push(list);
    assert(elt != NULL);
    elt->id = i + 1;
    elt->value = (i + 1) * 100;
  }
  test_element_t *first = list->part.elts;
  test_element_t *second = list->last->elts;
  assert(first[0].id == 1);
  assert(first[0].value == 100);
  assert(first[1].id == 2);
  assert(first[1].value == 200);
  assert(second[0].id == 3);
  assert(second[0].value == 300);
  rgx_arena_destroy(arena);
  puts("test_list_push_struct_elements: SUCCESS");
}

int
main(void)
{
  test_list_create();
  test_list_push_single();
  test_list_push_to_capacity();
  test_list_push_grow();
  test_list_push_multiple_parts();
  test_list_push_struct_elements();
  puts("main: All list tests passed!");
  return 0;
}

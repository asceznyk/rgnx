#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <rgx_core.h>

static void
test_init(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  assert(arena->active != NULL);
  assert(arena->active->size == RGX_ARENA_BLOCK_SIZE);
  assert(arena->active->used == 0);
  assert(arena->mdirect == NULL);
  rgx_arena_destroy(arena);
  puts("test_init: SUCCESS");
}

static void
test_basic_allocation(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  u_char *a = rgx_arena_alloc(arena, 32);
  u_char *b = rgx_arena_alloc(arena, 64);
  assert(a != NULL);
  assert(b != NULL);
  assert(a != b);
  memset(a, 'A', 32);
  memset(b, 'B', 64);
  for (size_t i = 0; i < 32; i++) assert(a[i] == 'A');
  for (size_t i = 0; i < 64; i++) assert(b[i] == 'B');
  rgx_arena_destroy(arena);
  puts("test_basic_allocation: SUCCESS");
}

static void
test_alignment(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  for (size_t i = 0; i < 16; i++) {
    u_char *ptr = rgx_arena_alloc(arena, i + 1);
    assert(ptr != NULL);
    assert((uintptr_t)ptr % RGX_ARENA_ALIGNMENT == 0);
  }
  rgx_arena_destroy(arena);
  puts("test_alignment: SUCCESS");
}

static void
test_chunk_growth(void)
{
  rgx_arena_t *arena = rgx_arena_init(128);
  assert(arena != NULL);
  size_t initial_chunks = 0;
  for (rgx_chunk_t *chunk = arena->active; chunk != NULL; chunk = chunk->next) initial_chunks++;
  for (size_t i = 0; i < 32; i++) {
    u_char *ptr = rgx_arena_alloc(arena, 64);
    assert(ptr != NULL);
    memset(ptr, (int)i, 64);
  }
  size_t final_chunks = 0;
  for (rgx_chunk_t *chunk = arena->active; chunk != NULL; chunk = chunk->next) final_chunks++;
  assert(final_chunks > initial_chunks);
  rgx_arena_destroy(arena);
  puts("test_chunk_growth: SUCCESS");
}

static void
test_direct_allocation(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  size_t size = RGX_ARENA_BLOCK_SIZE * 2;
  u_char *ptr = rgx_arena_alloc(arena, size);
  assert(ptr != NULL);
  assert(arena->mdirect != NULL);
  memset(ptr, 0xA5, size);
  for (size_t i = 0; i < size; i++) assert(ptr[i] == 0xA5);
  rgx_arena_destroy(arena);
  puts("test_direct_allocation: SUCCESS");
}

static void
test_multiple_directs(void)
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  assert(arena != NULL);
  size_t size = RGX_ARENA_BLOCK_SIZE * 2;
  u_char *a = rgx_arena_alloc(arena, size);
  u_char *b = rgx_arena_alloc(arena, size + 1);
  assert(a != NULL);
  assert(b != NULL);
  assert(a != b);
  assert(arena->mdirect != NULL);
  assert(arena->mdirect->next != NULL);
  memset(a, 'A', size);
  memset(b, 'B', size + 1);
  assert(a[0] == 'A');
  assert(b[0] == 'B');
  rgx_arena_destroy(arena);
  puts("test_multiple_directs: SUCCESS");
}

static void
test_chunk_metadata(void)
{
  rgx_arena_t *arena = rgx_arena_init(128);
  assert(arena != NULL);
  rgx_chunk_t *initial = arena->active;
  assert(initial != NULL);
  u_char *ptr = rgx_arena_alloc(arena, 96);
  assert(ptr != NULL);
  assert(initial->used > 0);
  assert(initial->used <= initial->size);
  rgx_arena_destroy(arena);
  puts("test_chunk_metadata: SUCCESS");
}

int
main(void)
{
  test_init();
  test_basic_allocation();
  test_alignment();
  test_chunk_growth();
  test_direct_allocation();
  test_multiple_directs();
  test_chunk_metadata();
  puts("main: All arena tests passed!");
  return 0;
}


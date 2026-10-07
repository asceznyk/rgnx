#include <rgx_core.h>

rgx_arena_t *
rgx_arena_init(size_t block_size)
{
  rgx_arena_t *arena = rgx_memalign(RGX_ARENA_ALIGNMENT, sizeof(*arena));
  if (!arena) {
    rgx_log_error("rgx_arena_init: rgx_memalign(arena) FAILED");
    return NULL;
  }
  rgx_block_t *block = rgx_malloc(sizeof(*block));
  if (!block) {
    rgx_log_error("rgx_arena_init: rgx_malloc(sizeof(*block)) FAILED");
    rgx_free(arena);
    return NULL;
  }
  block->size = block_size;
  block->used = 0;
  block->start = rgx_malloc(block_size);
  if (!block->start) {
    rgx_log_error("rgx_arena_init: rgx_malloc(block_size) FAILED");
    rgx_free(block);
    rgx_free(arena);
    return NULL;
  }
  block->prev = NULL;
  arena->block_size = block_size;
  arena->current = block;
  return arena;
}



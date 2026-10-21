#include <rgx_core.h>

static size_t rgx_align_up(size_t n, size_t align);

static size_t
rgx_align_up(size_t n, size_t align)
{
  return (n + align - 1) & ~(align - 1);
}

rgx_arena_t *
rgx_arena_init(size_t block_size)
{
  rgx_arena_t *arena = rgx_malloc(sizeof(*arena));
  if (!arena) {
    rgx_log_error("rgx_arena_init: rgx_memalign(arena) FAILED");
    return NULL;
  }
  rgx_chunk_t *chunk = rgx_malloc(sizeof(*chunk));
  if (!chunk) {
    rgx_log_error("rgx_arena_init: rgx_malloc(sizeof(*chunk)) FAILED");
    rgx_free(arena);
    return NULL;
  }
  chunk->size = block_size;
  chunk->used = 0;
  chunk->start = rgx_malloc(block_size);
  if (!chunk->start) {
    rgx_log_error("rgx_arena_init: rgx_malloc(block_size) FAILED");
    rgx_free(chunk);
    rgx_free(arena);
    return NULL;
  }
  chunk->next = NULL;
  arena->active = chunk;
  arena->mdirect = NULL;
  return arena;
}

int
rgx_arena_add_chunk(rgx_arena_t *arena)
{
  rgx_chunk_t *active = arena->active;
  rgx_chunk_t *new = rgx_malloc(sizeof(*new));
  if (!new) {
    rgx_log_error("rgx_arena_add_chunk: rgx_malloc FAILED");
    return -1;
  }
  new->size = active->size;
  new->used = 0;
  new->start = rgx_malloc(active->size);
  if (!new->start) {
    rgx_log_error("rgx_arena_add_chunk: rgx_malloc FAILED");
    rgx_free(new);
    return -1;
  }
  new->next = active;
  arena->active = new;
  return 0;
}

int
rgx_arena_add_overflow(rgx_arena_t *arena, size_t size)
{
  rgx_overflow_t *new = rgx_malloc(sizeof(*new));
  if (!new) {
    rgx_log_error("rgx_arena_add_overflow: rgx_malloc FAILED");
    return -1;
  }
  new->addr = rgx_malloc(size);
  if (!new->addr) {
    rgx_log_error("rgx_arena_add_overflow: rgx_malloc FAILED");
    rgx_free(new);
    return -1;
  }
  new->next = arena->mdirect;
  arena->mdirect = new;
  return 0;
}

u_char *
rgx_arena_alloc(rgx_arena_t *arena, size_t size)
{
  rgx_chunk_t *active = arena->active;
  if (active->size < size) {
    rgx_log_info("rgx_arena_alloc: passing to rgx_overflow_t..");
    if (rgx_arena_add_overflow(arena, size) < 0) {
      rgx_log_error("rgx_arena_alloc: rgx_arena_add_overflow FAILED");
      return NULL;
    }
    u_char *p = arena->mdirect->addr;
    return p;
  }
  size_t offset = rgx_align_up(active->used, RGX_ARENA_ALIGNMENT);
  if (offset > active->size || size > active->size - offset) {
    rgx_log_info("rgx_arena_alloc: remaining size < size");
    if (rgx_arena_add_chunk(arena) < 0) {
      rgx_log_error("rgx_arena_alloc: rgx_arena_add_chunk FAILED");
      return NULL;
    }
    active = arena->active;
    offset = rgx_align_up(active->used, RGX_ARENA_ALIGNMENT);
  }
  rgx_log_info(
    "rgx_arena_alloc: used = %zu, offset = %zu",
    active->used, offset
  );
  u_char *p = active->start + offset;
  active->used = offset + size;
  return p;
}

void
rgx_arena_destroy(rgx_arena_t *arena)
{
  rgx_chunk_t *active = arena->active;
  while (active) {
    rgx_chunk_t *p = active;
    active = active->next;
    rgx_free(p->start);
    rgx_free(p);
  }
  rgx_overflow_t *mdirect = arena->mdirect;
  while (mdirect) {
    rgx_overflow_t *p = mdirect;
    mdirect = mdirect->next;
    rgx_free(p->addr);
    rgx_free(p);
  }
  rgx_free(arena);
}



#ifndef RGX_ARENA_H
#define RGX_ARENA_H

#include <rgx_core.h>

#define RGX_ARENA_ALIGNMENT 16
#define RGX_ARENA_BLOCK_SIZE 1024

typedef struct rgx_chunk {
  size_t size;
  size_t used;
  u_char *start;
  struct rgx_chunk *next;
} rgx_chunk_t;

typedef struct rgx_overflow {
  u_char *addr;
  struct rgx_overflow *next;
} rgx_overflow_t;

typedef struct rgx_arena {
  rgx_chunk_t *active;
  rgx_overflow_t *mdirect;
} rgx_arena_t;

rgx_arena_t *rgx_arena_init(size_t block_size);
int rgx_arena_add_chunk(rgx_arena_t *arena);
int rgx_arena_add_overflow(rgx_arena_t *arena, size_t size);
u_char * rgx_arena_alloc(rgx_arena_t *arena, size_t size);

#endif





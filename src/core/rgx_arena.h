#ifndef RGX_ARENA_H
#define RGX_ARENA_H

#include <rgx_core.h>

#define RGX_ARENA_ALIGNMENT 16
#define RGX_ARENA_BLOCK_SIZE 1024

typedef struct rgx_block {
  size_t size;
  size_t used;
  u_char *start;
  struct rgx_block *prev;
} rgx_block_t;

typedef struct rgx_large_block {
  u_char *addr;
  struct rgx_large_block *next;
} rgx_large_block_t;

typedef struct rgx_arena {
  size_t block_size;
  rgx_block_t *current;
  rgx_large_block_t *large_blocks;
} rgx_arena_t;

rgx_arena_t *rgx_arena_init(size_t block_size);

#endif





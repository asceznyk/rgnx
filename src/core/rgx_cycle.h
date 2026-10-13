#ifndef RGX_CYCLE_H
#define RGX_CYCLE_H

#include <rgx_core.h>

typedef struct rgx_cycle_s {
  rgx_arena_t *arena;
  rgx_listening_t *listening;
  rgx_connection_t *connections;
  rgx_uint_t connection_n;
} rgx_cycle_t;

#endif


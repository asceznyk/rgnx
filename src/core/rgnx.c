#include <rgx_core.h>
#include <rgnx.h>

int
main(void)
{
  rgx_log_info("main: RGNX version = %s", RGNX_VER);
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  u_char *p = rgx_arena_alloc(arena, 20);
  u_char *q = rgx_arena_alloc(arena, 20);
  u_char *r = rgx_arena_alloc(arena, 4096);
  u_char *s = rgx_arena_alloc(arena, 1000);
  rgx_log_info("main: p = %p, q = %p, r = %p, s = %p", p, q, r, s);
  return 0;
}



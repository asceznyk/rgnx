#include <rgx_core.h>
#include <rgnx.h>

int
main(void)
{
  rgx_log_info("main: RGNX version = %s", RGNX_VER);
  /*rgx_uint_t x = 5;
  rgx_log_info("main: x = %zu", x);
  void *p = rgx_calloc(8);
  rgx_log_info("main: p = %p", p);*/
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  rgx_log_info("main: arena = %p", arena);
  return 0;
}



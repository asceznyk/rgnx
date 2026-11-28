#include <rgx_core.h>

static void
test_read_conf_file()
{
  rgx_arena_t *arena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  const char *cpath = "/home/asceznyk/programming/misc/mock_proxy/test.conf";
  rgx_str_t path = {
    .data = (u_char *) cpath,
    .len = rgx_strlen(cpath)
  };
  rgx_conf_root_t *root = (void *)rgx_arena_alloc(
    arena,
    sizeof(rgx_conf_root_t)
  );
  rgx_array_t *args = rgx_array_create(arena, 10, sizeof(rgx_str_t));
  rgx_conf_t conf = {
    .ctx = RGX_CONF_CTX_MAIN,
    .cptr = root,
    .root = root,
    .args = args,
    .conf_file = NULL,
    .arena = arena
  };
  if (rgx_conf_parse(&conf, &path) != RGX_CONF_OK) {
    rgx_arena_destroy(arena);
    return;
  }
  rgx_arena_destroy(arena);
}

int
main(void)
{
  test_read_conf_file();
  return 0;
}


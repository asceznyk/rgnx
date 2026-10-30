#include <rgx_core.h>

static void
test_read_conf_file()
{
  const char *cpath = "/home/asceznyk/programming/misc/mock_proxy/rgnx.conf";
  rgx_str_t path = {
    .data = cpath,
    .len = sizeof(cpath)-1
  };
  rgx_conf_tree_t *tree = rgx_conf_tree_init(RGX_ARENA_BLOCK_SIZE);
  rgx_conf_file_read(tree, &path);
  rgx_conf_tree_destroy(tree);
}

int
main(void)
{
  test_read_conf_file();
  return 0;
}


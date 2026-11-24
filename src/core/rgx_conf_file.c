#include <rgx_core.h>

rgx_conf_root_t *
rgx_conf_init(size_t block_size)
{
  rgx_arena_t *arena = rgx_arena_init(block_size);
  if (arena == NULL) {
    rgx_log_error("rgx_conf_init: rgx_arena_init FAILED!");
    return NULL;
  }
  rgx_conf_root_t *root = (void *)rgx_arena_alloc(
    arena,
    sizeof(rgx_conf_root_t)
  );
  if (root == NULL) {
    rgx_log_error("rgx_conf_init: rgx_arena_alloc FAILED!");
    rgx_arena_destroy(arena);
    return NULL;
  }
  root->arena = arena;
  return root;
}

void
rgx_conf_destroy(rgx_conf_root_t *root)
{
  assert(root != NULL);
  assert(root->arena != NULL);
  rgx_arena_destroy(root->arena);
  root = NULL;
}

char *
rgx_conf_parse(rgx_conf_t *conf, rgx_str_t *filename)
{
  rgx_fd_t fd;
  enum {
    parse_file = 0,
    parse_block,
    parse_param
  } type;
  if (filename) {
    fd = rgx_open_file(filename->data, RGX_FILE_RDONLY, RGX_FILE_OPEN, 0);
    rgx_log_info("rgx_conf_parse: fd = %d", fd);
    if (fd == RGX_INVALID_FILE) {
      rgx_log_error("rgx_conf_parse: INVALID file");
      return RGX_CONF_ERROR;
    }
  } //TODO: more stuff!
  return RGX_CONF_OK;
}



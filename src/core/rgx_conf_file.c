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
rgx_conf_parse(rgx_conf_root_t *root, rgx_str_t *filename)
{
  //TODO: a lot of crap!
  return RGX_CONF_OK;
}

/*rgx_int_t
rgx_conf_file_read(rgx_conf_root_t *tree, rgx_str_t *path)
{
  assert(tree != NULL);
  FILE *fp;
  u_char *filename = path->data;
  fp = fopen((const char *)(filename), "r");
  if (fp == NULL) {
    rgx_log_error(
      "rgx_conf_file_read: error opening config file = %s!",
      filename
    );
    return RGX_ERROR;
  }
  if (rgx_read_conf_tree(tree, fp) == RGX_ERROR) {
    fclose(fp);
    return RGX_ERROR;
  }
  fclose(fp);
  return RGX_OK;
}*/


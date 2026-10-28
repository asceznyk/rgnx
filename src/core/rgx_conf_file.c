#include <rgx_core.h>

static rgx_conf_tree_t *rgx_init_conf_tree(size_t block_size);
static rgx_int_t rgx_parse_conf_tree(rgx_conf_tree_t *tree, FILE *fp);

static rgx_conf_tree_t *
rgx_init_conf_tree(size_t block_size)
{
  rgx_arena_t *arena = rgx_arena_init(block_size);
  if (arena == NULL) {
    rgx_log_error("rgx_init_conf_tree: rgx_arena_init FAILED!");
    return NULL;
  }
  rgx_conf_tree_t *tree = (void *)rgx_arena_alloc(
    arena,
    sizeof(rgx_conf_tree_t)
  );
  if (tree == NULL) {
    rgx_log_error("rgx_init_conf_tree: rgx_arena_alloc FAILED!");
    return NULL;
  }
  tree->arena = arena;
  return tree;
}

static rgx_int_t
rgx_parse_conf_tree(rgx_conf_tree_t *tree, FILE *fp)
{
  u_char ch;
  //TODO: write this crap here!
  return RGX_OK;
}

rgx_conf_tree_t *
rgx_read_conf_file(rgx_str_t *filepath)
{
  FILE *fp;
  rgx_arena_t *arena;
  fp = fopen((const char *)(filepath->data), "r");
  if (fp == NULL) {
    rgx_log_error(
      "rgx_read_conf_file: error opening config file = %s!",
      filepath->data
    );
    fclose(fp);
    return NULL;
  }
  rgx_conf_tree_t *tree = rgx_init_conf_tree(RGX_ARENA_BLOCK_SIZE);
  if (tree == NULL) {
    fclose(fp);
    return NULL;
  }
  if (rgx_parse_conf_tree(tree, fp) == RGX_ERROR) {
    fclose(fp);
    return NULL;
  }
  fclose(fp);
  return tree;
}


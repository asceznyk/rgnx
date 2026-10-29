#include <rgx_core.h>

static rgx_conf_tree_t *rgx_init_conf_tree(size_t block_size);
static rgx_array_t *rgx_lex_conf_file(FILE *fp);
static rgx_int_t rgx_parse_conf_tree(
  rgx_conf_tree_t *tree, rgx_array_t *tokens
);
static rgx_int_t rgx_read_conf_tree(rgx_conf_tree_t *tree, FILE *fp);

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

static rgx_array_t *
rgx_lex_conf_file(FILE *fp)
{
  u_char ch;
  rgx_token_buf_t tbuf = {0};
  tbuf.len = 0;
  tbuf.capacity = RGX_CONF_TOKEN_MAX;
  tbuf.data = rgx_malloc(RGX_CONF_TOKEN_MAX * sizeof(u_char));
  rgx_arena_t *tarena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  rgx_array_t *tokens = rgx_array_create(tarena, 1, sizeof(rgx_conf_token_t));
  while ((ch = fgetc(fp)) != EOF) {
    if (tbuf.len >= tbuf.capacity)
      return NULL;
    if (ch == ';') {
      rgx_conf_token_t *token = rgx_array_push(tokens);
      token->type = RGX_CONF_TOKEN_SEMICOLON;
      token->value = (rgx_str_t) {
        .data = (u_char *) (";"),
        .len = 1
      };
      continue;
    }
    if (ch == '{' || ch == '}') {
      rgx_conf_token_t *token = rgx_array_push(tokens);
      token->type = (ch == '{') ? RGX_CONF_TOKEN_LBRACE : RGX_CONF_TOKEN_RBRACE;
      token->value = (rgx_str_t) {
        .data = (u_char *) ((ch == '{') ? "{" : "}"),
        .len = 1
      };
      continue;
    }
    if (ch == ' ' || ch == CR || ch == LF) {
      rgx_log_info(
        "rgx_read_conf_tree: tbuf.data = %.*s && tbuf.len = %zu",
        (int)tbuf.len,
        tbuf.data,
        tbuf.len
      );
      if (tbuf.len == 0)
        continue;
      rgx_conf_token_t *token = rgx_array_push(tokens);
      token->type = RGX_CONF_TOKEN_WORD;
      token->value = (rgx_str_t) {
        .data = tbuf.data,
        .len = tbuf.len
      };
      tbuf.len = 0;
      continue;
    }
    tbuf.data[tbuf.len++] = ch;
  }
  rgx_free(tbuf.data);
  return tokens;
}

static rgx_int_t
rgx_read_conf_tree(rgx_conf_tree_t *tree, FILE *fp)
{
  rgx_array_t *tokens = rgx_lex_conf_file(fp);
  //tree->http = (rgx_http_conf_t){0};
  return RGX_OK;
}

rgx_conf_tree_t *
rgx_read_conf_file(rgx_str_t *path)
{
  FILE *fp;
  rgx_arena_t *arena;
  u_char *filename = path->data;
  fp = fopen((const char *)(filename), "r");
  if (fp == NULL) {
    rgx_log_error(
      "rgx_read_conf_file: error opening config file = %s!",
      filename
    );
    fclose(fp);
    return NULL;
  }
  rgx_conf_tree_t *tree = rgx_init_conf_tree(RGX_ARENA_BLOCK_SIZE);
  if (tree == NULL) {
    fclose(fp);
    return NULL;
  }
  if (rgx_read_conf_tree(tree, fp) == RGX_ERROR) {
    fclose(fp);
    return NULL;
  }
  fclose(fp);
  return tree;
}


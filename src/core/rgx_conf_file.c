#include <rgx_core.h>

static rgx_int_t rgx_lex_conf_file(
  rgx_array_t *tokens, rgx_arena_t *arena, FILE *fp
);
static rgx_int_t rgx_parse_conf_block(
  void *root, rgx_array_t *tokens, rgx_uint_t *i
);
static rgx_int_t rgx_parse_conf_tree(
  rgx_conf_tree_t *tree, rgx_array_t *tokens
);
static rgx_int_t rgx_read_conf_tree(rgx_conf_tree_t *tree, FILE *fp);

static rgx_int_t
rgx_lex_conf_file(rgx_array_t *tokens, rgx_arena_t *arena, FILE *fp)
{
  int ch;
  rgx_token_buf_t tbuf = {0};
  tbuf.len = 0;
  tbuf.capacity = RGX_CONF_TOKEN_MAX;
  tbuf.data = rgx_malloc(RGX_CONF_TOKEN_MAX * sizeof(u_char));
  while ((ch = fgetc(fp)) != EOF) {
    if (tbuf.len >= tbuf.capacity)
      return RGX_ERROR;
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
        "rgx_lex_conf_file: tbuf.data = %.*s && tbuf.len = %zu",
        (int)tbuf.len,
        tbuf.data,
        tbuf.len
      );
      if (tbuf.len == 0)
        continue;
      rgx_conf_token_t *token = rgx_array_push(tokens);
      token->type = RGX_CONF_TOKEN_WORD;
      token->value = (rgx_str_t){0};
      token->value.data = rgx_arena_alloc(arena, tbuf.len);
      rgx_memcpy(token->value.data, tbuf.data, tbuf.len);
      token->value.len = tbuf.len;
      tbuf.len = 0;
      continue;
    }
    tbuf.data[tbuf.len++] = (u_char) ch;
  }
  rgx_log_info("rgx_lex_conf_file: end of loop!");
  rgx_conf_token_t *token = rgx_array_push(tokens);
  token->type = RGX_CONF_TOKEN_EOF;
  token->value = (rgx_str_t){0};
  rgx_free(tbuf.data);
  return RGX_OK;
}

static rgx_int_t
rgx_parse_conf_block(void *root, rgx_array_t *tokens, rgx_uint_t *i)
{
  rgx_conf_token_t *elts = ((rgx_conf_token_t *)tokens->elts);
  rgx_conf_token_t token = elts[*i];
  rgx_log_info(
    "rgx_parse_conf_tree: token = %.*s, type = %d",
    (int)token.value.len,
    token.value.data,
    token.type
  );
  /*if (rgx_memcmp("http", token.value.data, token.value.len) == 0) {
    *i += 1;
    token = elts[*i];
    if (token.type != RGX_CONF_TOKEN_LBRACE) {
      rgx_log_error("rgx_parse_conf_block: no left brace after `http`!");
      return RGX_ERROR;
    }
    root->http = (rgx_http_conf_t){0};
  }*/
  *i += 1;
  return RGX_OK;
}

static rgx_int_t
rgx_parse_conf_tree(rgx_conf_tree_t *tree, rgx_array_t *tokens)
{
  rgx_uint_t i = 0;
  rgx_conf_token_t *elts = tokens->elts;
  while (i < tokens->nelts) {
    if (rgx_parse_conf_block(tree, tokens, &i) == RGX_ERROR) {
      rgx_log_error("rgx_parse_conf_tree: rgx_parse_conf_block FAILED!");
      return RGX_ERROR;
    }
    if (elts[i].type == RGX_CONF_TOKEN_EOF)
      break;
  }
  return RGX_OK;
}

static rgx_int_t
rgx_read_conf_tree(rgx_conf_tree_t *tree, FILE *fp)
{
  rgx_arena_t *tarena, *varena;
  tarena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  varena = rgx_arena_init(RGX_ARENA_BLOCK_SIZE);
  rgx_array_t *tokens = rgx_array_create(tarena, 1, sizeof(rgx_conf_token_t));
  if (rgx_lex_conf_file(tokens, varena, fp) == RGX_ERROR) {
    rgx_log_error("rgx_read_conf_tree: rgx_lex_conf_file FAILED!");
    return RGX_ERROR;
  }
  if (rgx_parse_conf_tree(tree, tokens) == RGX_ERROR) {
    rgx_log_error("rgx_read_conf_tree: rgx_parse_conf_tree FAILED!");
    return RGX_ERROR;
  }
  rgx_arena_destroy(tarena);
  rgx_arena_destroy(varena);
  return RGX_OK;
}

rgx_conf_tree_t *
rgx_conf_tree_init(size_t block_size)
{
  rgx_arena_t *arena = rgx_arena_init(block_size);
  if (arena == NULL) {
    rgx_log_error("rgx_conf_tree_init: rgx_arena_init FAILED!");
    return NULL;
  }
  rgx_conf_tree_t *tree = (void *)rgx_arena_alloc(
    arena,
    sizeof(rgx_conf_tree_t)
  );
  if (tree == NULL) {
    rgx_log_error("rgx_conf_tree_init: rgx_arena_alloc FAILED!");
    return NULL;
  }
  tree->arena = arena;
  return tree;
}

void
rgx_conf_tree_destroy(rgx_conf_tree_t *tree)
{
  assert(tree != NULL);
  assert(tree->arena != NULL);
  rgx_arena_destroy(tree->arena);
  tree = NULL;
}

rgx_int_t
rgx_conf_file_read(rgx_conf_tree_t *tree, rgx_str_t *path)
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
    fclose(fp);
    return RGX_ERROR;
  }
  if (rgx_read_conf_tree(tree, fp) == RGX_ERROR) {
    fclose(fp);
    return RGX_ERROR;
  }
  fclose(fp);
  return RGX_OK;
}


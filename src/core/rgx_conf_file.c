#include <rgx_core.h>

static void rgx_log_tokens(rgx_array_t *tokens);
static rgx_int_t rgx_add_token(
  rgx_array_t *tokens, rgx_arena_t *arena, rgx_token_buf_t *tbuf
);
static rgx_int_t rgx_lex_conf_file(
  rgx_array_t *tokens, rgx_arena_t *arena, FILE *fp
);
static rgx_int_t rgx_token_compare(char *directive, rgx_conf_token_t token);
static rgx_int_t rgx_parse_single_line(
  char *directive, rgx_conf_parser_t *state
);
static rgx_int_t rgx_parse_host_port(
  rgx_upserver_conf_t *upserver, rgx_conf_parser_t *state, rgx_str_t addr
);
static rgx_int_t rgx_parse_upstream_block(
  rgx_upstream_conf_t *upstream, rgx_conf_parser_t *state
);
static rgx_int_t rgx_parse_location_block(
  rgx_location_conf_t *location, rgx_conf_parser_t *state
);
static rgx_int_t rgx_parse_server_block(
  rgx_server_conf_t *server, rgx_conf_parser_t *state
);
static rgx_int_t rgx_parse_http_block(
  rgx_http_conf_t *http, rgx_conf_parser_t *state
);
static rgx_int_t rgx_parse_main_block(
  rgx_conf_tree_t *root, rgx_conf_parser_t *state
);
static rgx_int_t rgx_parse_conf_block(
  void *root, rgx_arena_t *arena, rgx_conf_ctx_t ctx,
  rgx_array_t *tokens, rgx_uint_t *i
);
static rgx_int_t rgx_parse_conf_tree(
  rgx_conf_tree_t *tree, rgx_array_t *tokens
);
static rgx_int_t rgx_read_conf_tree(rgx_conf_tree_t *tree, FILE *fp);

static void
rgx_log_tokens(rgx_array_t *tokens)
{
  rgx_uint_t i = 0;
  rgx_conf_token_t *elts = tokens->elts;
  while (i < tokens->nelts) {
    rgx_log_info(
      "rgx_log_tokens: type = %d token = %.*s -> i = %zu",
      elts[i].type, (int)elts[i].value.len, elts[i].value.data, i
    );
    i++;
  }
}

static rgx_int_t
rgx_add_token(rgx_array_t *tokens, rgx_arena_t *arena, rgx_token_buf_t *tbuf)
{
  rgx_conf_token_t *token = rgx_array_push(tokens);
  token->type = RGX_CONF_TOKEN_WORD;
  token->value = (rgx_str_t){0};
  token->value.data = rgx_arena_alloc(arena, tbuf->len);
  rgx_memcpy(token->value.data, tbuf->data, tbuf->len);
  token->value.len = tbuf->len;
  tbuf->len = 0;
  return RGX_OK;
}

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
      if (tbuf.len > 0) {
        rgx_add_token(tokens, arena, &tbuf);
      }
      rgx_conf_token_t *token = rgx_array_push(tokens);
      token->type = RGX_CONF_TOKEN_SEMICOLON;
      token->value = (rgx_str_t) {
        .data = (u_char *) (";"),
        .len = 1
      };
      continue;
    }
    if (ch == '{' || ch == '}') {
      if (tbuf.len > 0) {
        rgx_add_token(tokens, arena, &tbuf);
      }
      rgx_conf_token_t *token = rgx_array_push(tokens);
      token->type = (ch == '{') ? RGX_CONF_TOKEN_LBRACE : RGX_CONF_TOKEN_RBRACE;
      token->value = (rgx_str_t) {
        .data = (u_char *) ((ch == '{') ? "{" : "}"),
        .len = 1
      };
      continue;
    }
    if (ch == ' ' || ch == LF) {
      if (tbuf.len == 0)
        continue;
      rgx_add_token(tokens, arena, &tbuf);
      continue;
    }
    tbuf.data[tbuf.len++] = (u_char) ch;
  }
  rgx_conf_token_t *token = rgx_array_push(tokens);
  token->type = RGX_CONF_TOKEN_EOF;
  token->value = (rgx_str_t){0};
  rgx_free(tbuf.data);
  rgx_log_tokens(tokens);
  return RGX_OK;
}

static rgx_int_t
rgx_token_compare(char *directive, rgx_conf_token_t token)
{
  size_t dlen = strlen(directive);
  rgx_uint_t v = (
    token.value.len == dlen &&
    rgx_memcmp(token.value.data, directive, dlen) == 0
  );
  return (v == 0) ? RGX_DECLINED : RGX_OK;
}

static rgx_int_t
rgx_parse_single_line(char *directive, rgx_conf_parser_t *state)
{
  rgx_conf_token_t *elts = state->tokens->elts;
  rgx_conf_token_t token = elts[state->pos];
  if (rgx_token_compare(directive, token) != RGX_OK)
    return RGX_DECLINED;
  token = elts[++state->pos];
  if (token.type != RGX_CONF_TOKEN_WORD) {
    rgx_log_error("rgx_parse_single_line: no value present!");
    return RGX_ERROR;
  }
  token = elts[++state->pos];
  if (token.type != RGX_CONF_TOKEN_SEMICOLON) {
    rgx_log_error("rgx_parse_single_line: no `;` present!");
    return RGX_ERROR;
  }
  return RGX_OK;
}

static rgx_int_t
rgx_parse_host_port(
  rgx_upserver_conf_t *upserver, rgx_conf_parser_t *state, rgx_str_t addr
)
{
  rgx_int_t port;
  upserver->host = (rgx_str_t){0};
  u_char *colon = rgx_find_last(addr.data, addr.len, ':');
  if (!colon || colon == addr.data) {
    rgx_log_error("rgx_parse_host_port: `:` missing in address");
    return RGX_ERROR;
  }
  size_t host_len = colon - addr.data;
  u_char *end = addr.data + addr.len;
  u_char *sport = colon+1;
  if (sport == end) {
    rgx_log_error("rgx_parse_host_port: missing port number!");
    return RGX_ERROR;
  }
  if (
    (port = rgx_atoi(sport, (size_t)(end-sport))) == RGX_ERROR
  )
    return RGX_ERROR;
  if (port > 65535) {
    rgx_log_error("rgx_parse_host_port: port value > 65535");
    return RGX_ERROR;
  }
  if (port < 1) {
    rgx_log_error("rgx_parse_host_port: port is not valid!");
    return RGX_ERROR;
  };
  upserver->host.data = rgx_arena_alloc(state->arena, host_len);
  rgx_memcpy(upserver->host.data, addr.data, host_len);
  upserver->host.len = host_len;
  upserver->port = (rgx_uint_t)port;
  return RGX_OK;
}

static rgx_int_t
rgx_parse_upstream_block(
  rgx_upstream_conf_t *upstream, rgx_conf_parser_t *state
)
{
  rgx_int_t status, keepalive;
  rgx_conf_token_t *elts = state->tokens->elts;
  rgx_conf_token_t token = elts[state->pos];
  rgx_array_init(
    &upstream->servers, state->arena, 1, sizeof(rgx_upserver_conf_t)
  );
  if (token.type != RGX_CONF_TOKEN_LBRACE) {
    rgx_log_error("rgx_parse_upstream_block: `{` is missing!");
    return RGX_ERROR;
  }
  token = elts[++state->pos];
  while (
    token.type != RGX_CONF_TOKEN_EOF &&
    token.type != RGX_CONF_TOKEN_RBRACE
  ) {
    status = rgx_parse_single_line(RGX_CONF_DIRECTIVE_SERVER, state);
    if (status == RGX_ERROR) {
      rgx_log_error("rgx_parse_upstream_block: error in server line!");
      return RGX_ERROR;
    } else if (status == RGX_OK) {
      token = elts[--state->pos];
      rgx_upserver_conf_t *upserver = rgx_array_push(&upstream->servers);
      if (rgx_parse_host_port(upserver, state, token.value) != RGX_OK) {
        rgx_log_error("rgx_parse_upstream_block: host or port is not valid!");
        return RGX_ERROR;
      }
    }
    status = rgx_parse_single_line(RGX_CONF_DIRECTIVE_KEEPALIVE, state);
    if (status == RGX_ERROR) {
      rgx_log_error("rgx_parse_upstream_block: error in keepalive line!");
      return RGX_ERROR;
    }
    if (status == RGX_OK) {
      token = elts[--state->pos];
      if (
        (keepalive = rgx_atoi(token.value.data, token.value.len)) == RGX_ERROR
      ) {
        return RGX_ERROR;
      }
      upstream->keepalive = (rgx_uint_t)keepalive;
    }
    token = elts[++state->pos];
  }
  if (token.type == RGX_CONF_TOKEN_EOF) {
    rgx_log_error("rgx_parse_upstream_block: unexpected EOF");
    return RGX_ERROR;
  }
  return RGX_OK;
}

static rgx_int_t
rgx_parse_location_block(
  rgx_location_conf_t *location, rgx_conf_parser_t *state
)
{
  rgx_int_t status;
  rgx_arena_t *arena = state->arena;
  rgx_conf_token_t *elts = state->tokens->elts;
  rgx_conf_token_t token = elts[state->pos];
  rgx_conf_string_directive_t directives[] = {
    { "alias", &location->alias },
    { "proxy_pass", &location->proxy_pass },
    { "proxy_http_version", &location->proxy_http_version }
  };
  if (token.type != RGX_CONF_TOKEN_LBRACE) {
    rgx_log_error("rgx_parse_location_block: no `{` present!");
    return RGX_ERROR;
  }
  token = elts[++state->pos];
  while (
    token.type != RGX_CONF_TOKEN_EOF &&
    token.type != RGX_CONF_TOKEN_RBRACE
  ) {
    for (size_t i = 0; i < sizeof(directives) / sizeof(directives[0]); i++) {
      status = rgx_parse_single_line(directives[i].name, state);
      if (status == RGX_ERROR) {
        return RGX_ERROR;
      }
      if (status == RGX_OK) {
        token = elts[--state->pos];
        *directives[i].value = rgx_str_dup(arena, token.value);
        if (directives[i].value->data == NULL) {
          return RGX_ERROR;
        }
      }
    }
    token = elts[++state->pos];
  }
  if (token.type == RGX_CONF_TOKEN_EOF) {
    rgx_log_error("rgx_parse_location_block: unexpected EOF!");
    return RGX_ERROR;
  }
  return RGX_OK;
}

static rgx_int_t
rgx_parse_server_block(rgx_server_conf_t *server, rgx_conf_parser_t *state)
{
  rgx_int_t status, listen;
  rgx_arena_t *arena = state->arena;
  rgx_conf_token_t *elts = state->tokens->elts;
  rgx_conf_token_t token = elts[state->pos];
  rgx_array_init(&server->locations, arena, 1, sizeof(rgx_location_conf_t));
  if (token.type != RGX_CONF_TOKEN_LBRACE) {
    rgx_log_error("rgx_parse_server_block: no `{` present!");
    return RGX_ERROR;
  }
  token = elts[++state->pos];
  while (
    token.type != RGX_CONF_TOKEN_EOF &&
    token.type != RGX_CONF_TOKEN_RBRACE
    ) {
    status = rgx_parse_single_line("listen", state);
    if (status == RGX_ERROR) {
      rgx_log_error("rgx_parse_server_block: error in listen directive");
      return RGX_ERROR;
    }
    if (status == RGX_OK) {
      token = elts[--state->pos];
      if (
        (listen = rgx_atoi(token.value.data, token.value.len)) == RGX_ERROR
      ) {
        return RGX_ERROR;
      }
      server->port = (rgx_uint_t)listen;
    }
    if (rgx_token_compare(RGX_CONF_DIRECTIVE_LOCATION, token) == RGX_OK) {
      token = elts[++state->pos];
      if (token.type != RGX_CONF_TOKEN_WORD) {
        rgx_log_error("rgx_parse_server_block: no path in `location`!");
        return RGX_ERROR;
      }
      rgx_location_conf_t *loc = rgx_array_push(&server->locations);
      *loc = (rgx_location_conf_t){0};
      rgx_str_t path = rgx_str_dup(arena, token.value);
      loc->path = path;
      state->pos++;
      if (rgx_parse_location_block(loc, state) != RGX_OK) {
        rgx_log_error("rgx_parse_server_block: `location` block not valid!");
        return RGX_ERROR;
      }
    }
    token = elts[++state->pos];
  }
  if (token.type == RGX_CONF_TOKEN_EOF) {
    rgx_log_error("rgx_parse_server_block: unexpected EOF!");
    return RGX_ERROR;
  }
  return RGX_OK;
}

static rgx_int_t
rgx_parse_http_block(rgx_http_conf_t *http, rgx_conf_parser_t *state)
{
  rgx_arena_t *arena = state->arena;
  rgx_conf_token_t *elts = state->tokens->elts;
  rgx_conf_token_t token = elts[state->pos];
  if (token.type != RGX_CONF_TOKEN_LBRACE) {
    rgx_log_error("rgx_parse_conf_block: no `{` after `http`!");
    return RGX_ERROR;
  }
  token = elts[++state->pos];
  rgx_array_init(&http->upstreams, arena, 1, sizeof(rgx_upstream_conf_t));
  rgx_array_init(&http->servers, arena, 1, sizeof(rgx_server_conf_t));
  while (
    token.type != RGX_CONF_TOKEN_EOF &&
    token.type != RGX_CONF_TOKEN_RBRACE
  ) {
    if (rgx_token_compare(RGX_CONF_DIRECTIVE_UPSTREAM, token) == RGX_OK) {
      token = elts[++state->pos];
      if (token.type != RGX_CONF_TOKEN_WORD) {
        rgx_log_error("rgx_parse_conf_block: no name for `upstream`!");
        return RGX_ERROR;
      }
      rgx_str_t name = rgx_str_dup(arena, token.value);
      token = elts[++state->pos];
      rgx_upstream_conf_t *upstream = rgx_array_push(&http->upstreams);
      *upstream = (rgx_upstream_conf_t){0};
      upstream->name = name;
      if (rgx_parse_upstream_block(upstream, state) != RGX_OK) {
        rgx_log_error("rgx_parse_http_block: error in upstream config!");
        return RGX_ERROR;
      }
    }
    if (rgx_token_compare(RGX_CONF_DIRECTIVE_SERVER, token) == RGX_OK) {
      token = elts[++state->pos];
      rgx_server_conf_t *server = rgx_array_push(&http->servers);
      *server = (rgx_server_conf_t){0};
      if (rgx_parse_server_block(server, state) != RGX_OK) {
        rgx_log_error("rgx_parse_http_block: error in server config!");
        return RGX_ERROR;
      }
    }
    token = elts[++state->pos];
  }
  if (token.type == RGX_CONF_TOKEN_EOF) {
    rgx_log_error("rgx_parse_http_block: unexpected EOF!");
    return RGX_ERROR;
  }
  return RGX_OK;
}

static rgx_int_t
rgx_parse_main_block(rgx_conf_tree_t *root, rgx_conf_parser_t *state)
{
  rgx_conf_token_t *elts = state->tokens->elts;
  rgx_conf_token_t token = elts[state->pos];
  if (rgx_token_compare(RGX_CONF_DIRECTIVE_HTTP, token) == RGX_OK) {
    token = elts[++state->pos];
    root->http = (rgx_http_conf_t){0};
    if (rgx_parse_http_block(&root->http, state) != RGX_OK) {
      rgx_log_error("rgx_parse_main_block: http block has an error");
      return RGX_ERROR;
    }
  }
  return RGX_OK;
}

static rgx_int_t
rgx_parse_conf_tree(rgx_conf_tree_t *tree, rgx_array_t *tokens)
{
  rgx_conf_token_t *elts = tokens->elts;
  rgx_conf_parser_t *state = (void *)rgx_arena_alloc(
    tree->arena, sizeof(rgx_conf_parser_t)
  );
  state->arena = tree->arena;
  state->tokens = tokens;
  state->pos = 0;
  while (state->pos < tokens->nelts) {
    if (
      rgx_parse_main_block(tree, state) != RGX_OK
    ) {
      rgx_log_error("rgx_parse_conf_tree: rgx_parse_main_block FAILED!");
      return RGX_ERROR;
    }
    if (elts[state->pos].type == RGX_CONF_TOKEN_EOF)
      break;
    state->pos++;
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


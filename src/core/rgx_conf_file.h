#ifndef RGX_CONF_FILE_H
#define RGX_CONF_FILE_H

#include <rgx_core.h>

#define RGX_CONF_LIST_MIN 16
#define RGX_CONF_TOKEN_MAX 1024

#define RGX_CONF_DIRECTIVE_HTTP "http"
#define RGX_CONF_DIRECTIVE_SERVER "server"
#define RGX_CONF_DIRECTIVE_UPSTREAM "upstream"
#define RGX_CONF_DIRECTIVE_LOCATION "location"
#define RGX_CONF_DIRECTIVE_KEEPALIVE "keepalive"

typedef enum {
  RGX_CONF_TOKEN_WORD,
  RGX_CONF_TOKEN_LBRACE,
  RGX_CONF_TOKEN_RBRACE,
  RGX_CONF_TOKEN_SEMICOLON,
  RGX_CONF_TOKEN_EOF
} rgx_conf_token_type_t;

typedef struct {
  size_t len;
  size_t capacity;
  u_char *data;
} rgx_token_buf_t;

typedef struct {
  rgx_conf_token_type_t type;
  rgx_str_t value;
} rgx_conf_token_t;

typedef enum {
  RGX_CONF_CTX_MAIN,
  RGX_CONF_CTX_HTTP,
  RGX_CONF_CTX_UPSTREAM,
  RGX_CONF_CTX_SERVER,
  RGX_CONF_CTX_LOCATION,
  RGX_CONF_CTX_UPSERVER,
} rgx_conf_ctx_t;

typedef struct {
  rgx_array_t *tokens;
  rgx_uint_t pos;
  rgx_arena_t *arena;
} rgx_conf_parser_t;

typedef struct rgx_location_conf_s {
  rgx_str_t path;
  rgx_str_t alias;
  rgx_str_t proxy_pass;
  rgx_uint_t proxy_http_version;
} rgx_location_conf_t;

typedef struct rgx_upserver_conf_s {
  rgx_str_t host;
  rgx_uint_t port;
} rgx_upserver_conf_t;

typedef struct rgx_upstream_conf_s {
  rgx_str_t name;
  rgx_array_t servers;
  rgx_uint_t keepalive;
} rgx_upstream_conf_t;

typedef struct rgx_server_conf_s {
  rgx_uint_t port;
  rgx_int_t backlog;
  rgx_array_t locations;
} rgx_server_conf_t;

typedef struct rgx_http_conf_s {
  rgx_array_t upstreams;
  rgx_array_t servers;
} rgx_http_conf_t;

typedef struct rgx_conf_tree_s {
  rgx_http_conf_t http;
  rgx_arena_t *arena;
} rgx_conf_tree_t;

rgx_conf_tree_t *rgx_conf_tree_init(size_t block_size);
rgx_int_t rgx_conf_file_read(
  rgx_conf_tree_t *tree, rgx_str_t *filename
);
void rgx_conf_tree_destroy(rgx_conf_tree_t *tree);

#endif



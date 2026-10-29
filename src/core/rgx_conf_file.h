#ifndef RGX_CONF_FILE_H
#define RGX_CONF_FILE_H

#include <rgx_core.h>

#define RGX_CONF_TOKEN_MAX 1024

typedef enum {
  RGX_CONF_TOKEN_WORD,
  RGX_CONF_TOKEN_LBRACE,
  RGX_CONF_TOKEN_RBRACE,
  RGX_CONF_TOKEN_SEMICOLON,
  RGX_CONF_TOKEN_EOF
} rgx_conf_token_type_t;

typedef struct {
  rgx_conf_token_type_t type;
  rgx_str_t value;
} rgx_conf_token_t;

typedef struct rgx_location_conf_s {
  rgx_str_t path;
  rgx_str_t alias;
  rgx_str_t proxy_pass;
  rgx_uint_t proxy_http_version;
} rgx_location_conf_t;

typedef struct rgx_upstream_server_s {
  rgx_str_t host;
  rgx_uint_t port;
} rgx_upstream_server_t;

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

rgx_conf_tree_t *rgx_read_conf_file(rgx_str_t *filename);

#endif



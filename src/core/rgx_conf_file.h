#ifndef RGX_CONF_FILE_H
#define RGX_CONF_FILE_H

#include <rgx_core.h>

#define RGX_CONF_OK NULL
#define RGX_CONF_ERROR (void *) -1

#define RGX_CONF_BLOCK_START 1
#define RGX_CONF_BLOCK_DONE 2
#define RGX_CONF_FILE_DONE 3

typedef struct {
  rgx_array_t *tokens;
  rgx_uint_t pos;
  rgx_arena_t *arena;
} rgx_conf_parser_t;

typedef struct rgx_location_conf_s {
  rgx_str_t path;
  rgx_str_t alias;
  rgx_str_t proxy_pass;
  rgx_str_t proxy_http_version;
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

typedef struct rgx_conf_root_s {
  rgx_http_conf_t http;
  rgx_arena_t *arena;
} rgx_conf_root_t;

rgx_conf_root_t *rgx_conf_init(size_t block_size);
char *rgx_conf_parse(
  rgx_conf_root_t *root, rgx_str_t *filename
);
void rgx_conf_destroy(rgx_conf_root_t *root);

#endif



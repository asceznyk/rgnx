#ifndef RGX_CONF_FILE_H
#define RGX_CONF_FILE_H

#include <rgx_core.h>

#define RGX_CONF_OK NULL
#define RGX_CONF_ERROR (void *) -1

#define RGX_CONF_BLOCK_START 1
#define RGX_CONF_BLOCK_DONE 2
#define RGX_CONF_FILE_DONE 3

typedef enum {
  RGX_CONF_CTX_MAIN,
  RGX_CONF_CTX_HTTP,
  RGX_CONF_CTX_UPSTREAM,
  RGX_CONF_CTX_SERVER,
  RGX_CONF_CTX_SERVER_LOCATION,
  RGX_CONF_CTX_UPSTREAM_SERVER,
} rgx_conf_ctx_t;

typedef struct rgx_conf_location_s {
  rgx_str_t path;
  rgx_str_t alias;
  rgx_str_t proxy_pass;
  rgx_str_t proxy_http_version;
} rgx_conf_location_t;

typedef struct rgx_conf_upstream_server_s {
  rgx_str_t host;
  rgx_uint_t port;
} rgx_conf_upstream_server_t;

typedef struct rgx_conf_upstream_s {
  rgx_str_t name;
  rgx_array_t servers;
  rgx_uint_t keepalive;
} rgx_conf_upstream_t;

typedef struct rgx_conf_server_s {
  rgx_uint_t port;
  rgx_int_t backlog;
  rgx_array_t locations;
} rgx_conf_server_t;

typedef struct rgx_conf_http_s {
  rgx_array_t upstreams;
  rgx_array_t servers;
} rgx_conf_http_t;

typedef struct rgx_conf_root_s {
  rgx_conf_http_t http;
} rgx_conf_root_t;

typedef struct {
  rgx_file_t file;
  rgx_buf_t *buffer;
  rgx_buf_t *dump;
  rgx_uint_t line;
} rgx_conf_file_t;

struct rgx_conf_s {
  rgx_conf_ctx_t ctx;
  void *cptr;
  rgx_conf_root_t *root;
  rgx_array_t *args;
  rgx_conf_file_t *conf_file;
  rgx_arena_t *arena;
};

char *rgx_conf_parse(
  rgx_conf_t *conf, rgx_str_t *filename
);

#endif



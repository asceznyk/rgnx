#include <rgx_core.h>

static void
test_read_conf_file()
{
  const char *cpath = "/home/asceznyk/programming/misc/mock_proxy/test.conf";
  rgx_str_t path = {
    .data = (u_char *) cpath,
    .len = strlen(cpath)
  };
  rgx_conf_tree_t *tree = rgx_conf_tree_init(RGX_ARENA_BLOCK_SIZE);
  rgx_conf_file_read(tree, &path);
  rgx_http_conf_t http = tree->http;
  rgx_array_t *array = &http.upstreams;
  rgx_upstream_conf_t *elts = array->elts;
  for (rgx_uint_t i = 0; i < array->nelts; i++) {
    rgx_upstream_conf_t upstream = elts[i];
    rgx_log_info("test_read_conf_file: keepalive = %zu", upstream.keepalive);
    rgx_log_info(
      "test_read_conf_file: name = %.*s",
      (int)upstream.name.len, upstream.name.data
    );
    rgx_array_t *arr2 = &upstream.servers;
    rgx_upserver_conf_t *upservers = arr2->elts;
    for (rgx_uint_t j = 0; j < arr2->nelts; j++) {
      rgx_upserver_conf_t upserver = upservers[i];
      rgx_log_info(
        "test_read_conf_file: host = %.*s, port = %zu",
        (int)upserver.host.len, upserver.host.data, upserver.port
      );
    }
  }
  rgx_conf_tree_destroy(tree);
}

int
main(void)
{
  test_read_conf_file();
  return 0;
}


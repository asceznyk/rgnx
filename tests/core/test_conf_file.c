#include <rgx_core.h>

static void
test_read_conf_file()
{
  const char *cpath = "/home/asceznyk/programming/misc/mock_proxy/test.conf";
  rgx_str_t path = {
    .data = (u_char *) cpath,
    .len = rgx_strlen(cpath)
  };
  rgx_conf_tree_t *tree = rgx_conf_tree_init(RGX_ARENA_BLOCK_SIZE);
  rgx_conf_file_read(tree, &path);
  rgx_http_conf_t http = tree->http;
  rgx_upstream_conf_t *upstreams = http.upstreams.elts;
  for (rgx_uint_t i = 0; i < http.upstreams.nelts; i++) {
    rgx_upstream_conf_t upstream = upstreams[i];
    rgx_log_info(
      "test_read_conf_file: name = %.*s",
      (int)upstream.name.len, upstream.name.data
    );
    rgx_log_info("test_read_conf_file: keepalive = %zu", upstream.keepalive);
    rgx_upserver_conf_t *upservers = upstream.servers.elts;
    for (rgx_uint_t j = 0; j < upstream.servers.nelts; j++) {
      rgx_upserver_conf_t upserver = upservers[i];
      rgx_log_info(
        "test_read_conf_file: host = %.*s, port = %zu",
        (int)upserver.host.len, upserver.host.data, upserver.port
      );
    }
  }
  rgx_server_conf_t *servers = http.servers.elts;
  for (rgx_uint_t i = 0; i < http.servers.nelts; i++) {
    rgx_server_conf_t server = servers[i];
    rgx_log_info("test_read_conf_file: port = %zu", server.port);
    rgx_log_info("test_read_conf_file: backlog = %zu", server.backlog);
    rgx_location_conf_t *locations = server.locations.elts;
    for (rgx_uint_t j = 0; j < server.locations.nelts; j++) {
      rgx_location_conf_t location = locations[j];
      rgx_log_info(
        "test_read_conf_file: path = %.*s",
        (int)location.path.len, location.path.data
      );
      rgx_log_info(
        "test_read_conf_file: alias = %.*s",
        (int)location.alias.len, location.alias.data
      );
      rgx_log_info(
        "test_read_conf_file: proxy_pass = %.*s",
        (int)location.proxy_pass.len, location.proxy_pass.data
      );
      rgx_log_info(
        "test_read_conf_file: proxy_http_version = %.*s",
        (int)location.proxy_http_version.len, location.proxy_http_version.data
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


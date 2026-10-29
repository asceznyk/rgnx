#include <rgx_core.h>

static void
test_read_conf_file()
{
  const char *cpath = "/home/asceznyk/programming/misc/mock_proxy/rgnx.conf";
  rgx_str_t path = {
    .data = cpath,
    .len = sizeof(cpath)-1
  };
  rgx_read_conf_file(&path);
}

int
main(void)
{
  test_read_conf_file();
  return 0;
}


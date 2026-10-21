#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <rgx_core.h>

static void
test_epoll_init(void)
{
  rgx_cycle_t cycle = {0};
  cycle.connection_n = 1024;
  assert(rgx_epoll_init(&cycle) == RGX_OK);
  printf("test_epoll_init: epoll initialization succeeded\n");
}

static void
test_epoll_add_connection(void)
{
  rgx_cycle_t cycle = {0};
  rgx_connection_t c = {0};
  rgx_event_t read = {0};
  rgx_event_t write = {0};
  int pipefd[2];
  assert(rgx_epoll_init(&cycle) == RGX_OK);
  assert(pipe(pipefd) == 0);
  c.fd = pipefd[0];
  c.read = &read;
  c.write = &write;
  assert(rgx_epoll_add_connection(&c) == RGX_OK);
  assert(c.read->active == 1);
  assert(c.write->active == 1);
  printf("test_epoll_add_connection: connection registered successfully\n");
  close(pipefd[0]);
  close(pipefd[1]);
}

int
main(void)
{
  test_epoll_init();
  test_epoll_add_connection();
  printf("main: All epoll tests passed.\n");
  return 0;
}


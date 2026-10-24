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
  puts("test_epoll_init: SUCCESS");
}

static void
test_epoll_add_connection(void)
{
  rgx_cycle_t cycle = {0};
  rgx_connection_t conn = {0};
  rgx_event_t read = {0};
  rgx_event_t write = {0};
  int pipefd[2];
  assert(rgx_epoll_init(&cycle) == RGX_OK);
  assert(pipe(pipefd) == 0);
  conn.fd = pipefd[0];
  conn.read = &read;
  conn.write = &write;
  assert(rgx_epoll_add_connection(&conn) == RGX_OK);
  assert(conn.read->active == 1);
  assert(conn.write->active == 1);
  puts("test_epoll_add_connection: SUCCESS");
  close(pipefd[0]);
  close(pipefd[1]);
}

static void
test_epoll_del_connection(void)
{
  rgx_cycle_t cycle = {0};
  rgx_connection_t conn = {0};
  rgx_event_t read = {0};
  rgx_event_t write = {0};
  int pipefd[2];
  assert(rgx_epoll_init(&cycle) == RGX_OK);
  assert(pipe(pipefd) == 0);
  conn.fd = pipefd[0];
  conn.read = &read;
  conn.write = &write;
  read.active = 1;
  write.active = 1;
  assert(rgx_epoll_add_connection(&conn) == RGX_OK);
  assert(rgx_epoll_del_connection(&conn, 0) == RGX_OK);
  assert(read.active == 0);
  assert(write.active == 0);
  close(pipefd[0]);
  close(pipefd[1]);
  puts("test_epoll_del_connection: SUCCESS");
}

static void
test_epoll_del_connection_close_event(void)
{
  rgx_cycle_t cycle = {0};
  rgx_connection_t conn = {0};
  rgx_event_t read = {0};
  rgx_event_t write = {0};
  assert(rgx_epoll_init(&cycle) == RGX_OK);
  conn.read = &read;
  conn.write = &write;
  read.active = 1;
  write.active = 1;
  assert(rgx_epoll_del_connection(&conn, RGX_CLOSE_EVENT) == RGX_OK);
  assert(read.active == 0);
  assert(write.active == 0);
  puts("test_epoll_del_connection_close_event: SUCCESS");
}

static void
test_epoll_del_connection_failure(void)
{
  rgx_cycle_t cycle = {0};
  rgx_connection_t conn = {0};
  rgx_event_t read = {0};
  rgx_event_t write = {0};
  int pipefd[2];
  assert(rgx_epoll_init(&cycle) == RGX_OK);
  assert(pipe(pipefd) == 0);
  conn.fd = pipefd[0];
  conn.read = &read;
  conn.write = &write;
  read.active = 1;
  write.active = 1;
  assert(rgx_epoll_del_connection(&conn, 0) == RGX_ERROR);
  assert(read.active == 1);
  assert(write.active == 1);
  close(pipefd[0]);
  close(pipefd[1]);
  puts("test_epoll_del_connection_failure: SUCCESS");
}

int
main(void)
{
  test_epoll_init();
  test_epoll_add_connection();
  test_epoll_del_connection();
  test_epoll_del_connection_close_event();
  test_epoll_del_connection_failure();
  puts("main: All epoll tests passed!");
  return 0;
}


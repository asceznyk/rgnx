#include <rgx_core.h>

static int ep = -1;
static rgx_int_t rgx_epoll_init(rgx_cycle_t *cycle);

static rgx_int_t
rgx_epoll_init(rgx_cycle_t *cycle)
{
  if (ep == -1) {
    ep = epoll_create(cycle->connection_n/2);
    rgx_log_info("rgx_epoll_init: epoll_create worked!");
  }
  if (ep == -1) {
    rgx_log_error("rgx_epoll_init error from epoll_create!");
    return -1;
  }
  return ep;
}


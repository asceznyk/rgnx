#include <rgx_core.h>

static int ep = -1;
static struct epoll_event *event_list;
static rgx_uint_t nevents;

rgx_int_t
rgx_epoll_init(rgx_cycle_t *cycle)
{
  if (ep == -1) {
    ep = epoll_create(cycle->connection_n/2);
    if (ep == -1) {
      rgx_log_error("rgx_epoll_init error from epoll_create!");
      return RGX_ERROR;
    }
  }
  rgx_log_info("rgx_epoll_init: epoll_create OK.");
  return RGX_OK;
}

rgx_int_t
rgx_epoll_add_connection(rgx_connection_t *c)
{
  struct epoll_event ee;
  ee.events = EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLET;
  ee.data.ptr = (void *) ((uintptr_t) c | c->read->instance);
  rgx_log_info(
    "rgx_epoll_add_connection: attempting to add fd = %d events = %08XD",
    c->fd, ee.events
  );
  if (epoll_ctl(ep, EPOLL_CTL_ADD, c->fd, &ee) < 0) {
    rgx_log_error("rgx_epoll_add_connection: epoll_ctl FAILED!");
    return RGX_ERROR;
  }
  c->read->active = 1;
  c->write->active = 1;
  return RGX_OK;
}




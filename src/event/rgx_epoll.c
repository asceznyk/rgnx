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

void
rgx_epoll_done(rgx_cycle_t *cycle)
{
  if (close(ep) == -1) {
    rgx_log_error("rgx_epoll_done: close(%fd) FAILED!", ep);
  }
  ep = -1;
  rgx_free(event_list);
  event_list = NULL;
  nevents = 0;
}

rgx_int_t
rgx_epoll_add_connection(rgx_connection_t *conn)
{
  struct epoll_event ee;
  ee.events = EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLET;
  ee.data.ptr = (void *) ((uintptr_t) conn | conn->read->instance);
  rgx_log_info(
    "rgx_epoll_add_connection: attempting to add fd = %d events = %08XD",
    conn->fd, ee.events
  );
  if (epoll_ctl(ep, EPOLL_CTL_ADD, conn->fd, &ee) == -1) {
    rgx_log_error(
      "rgx_epoll_add_connection: epoll_ctl FAILED fd = %d!",
      conn->fd
    );
    return RGX_ERROR;
  }
  conn->read->active = 1;
  conn->write->active = 1;
  return RGX_OK;
}

rgx_int_t
rgx_epoll_del_connection(rgx_connection_t *conn, rgx_uint_t flags)
{
  struct epoll_event ee;
  if (flags & RGX_CLOSE_EVENT) {
    conn->read->active = 0;
    conn->write->active = 0;
    return RGX_OK;
  }
  ee.events = 0;
  ee.data.ptr = NULL;
  if (epoll_ctl(ep, EPOLL_CTL_DEL, conn->fd, &ee) == -1) {
    rgx_log_error(
      "rgx_epoll_del_connection: epoll_ctl FAILED fd = %d!",
      conn->fd
    );
    return RGX_ERROR;
  }
  conn->read->active = 0;
  conn->write->active = 0;
  return RGX_OK;
}




#ifndef RGX_EVENT_H
#define RGX_EVENT_H

#include <rgx_core.h>

#define RGX_CLOSE_EVENT 1

struct rgx_event_s {
  void *data;
  unsigned write:1;
  unsigned active:1;
  unsigned instance:1;
  unsigned ready:1;
  unsigned delayed:1;
  unsigned timedout:1;
  unsigned eof:1;
  unsigned pending_eof:1;
  unsigned error:1;
  int available;
  rgx_event_handler_pt handler;
};

rgx_int_t rgx_epoll_init(rgx_cycle_t *cycle);
void rgx_epoll_done(rgx_cycle_t *cycle);
rgx_int_t rgx_epoll_add_connection(rgx_connection_t *conn);
rgx_int_t rgx_epoll_del_connection(rgx_connection_t *conn, rgx_uint_t flags);
//rgx_int_t rgx_epoll_add_event(rgx_event_t *ev, rgx_int_t fd, rgx_uint_t events);
//int rgx_epoll_del_add_event(rgx_event_t *ev, int fd);
//int rgx_epoll_wait(int timeout);

void rgx_event_init(rgx_cycle_t *cycle);
void rgx_process_events(rgx_cycle_t *cycle);

#endif



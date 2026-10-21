#ifndef RGX_EPOLL_H
#define RGX_EPOLL_H

#include <rgx_core.h>

rgx_int_t rgx_epoll_init(rgx_cycle_t *cycle);
rgx_int_t rgx_epoll_add_connection(rgx_connection_t *c);
//rgx_int_t rgx_epoll_add_event(rgx_event_t *ev, rgx_int_t fd, rgx_uint_t events);
//int rgx_epoll_del_add_event(rgx_event_t *ev, int fd);
//int rgx_epoll_wait(int timeout);

#endif

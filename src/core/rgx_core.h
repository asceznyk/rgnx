#ifndef RGX_CORE_H
#define RGX_CORE_H

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define RGX_OK 0
#define RGX_ERROR -1
#define RGX_AGAIN -2
#define RGX_BUSY -3
#define RGX_DONE -4
#define RGX_DECLINED -5
#define RGX_ABORT -6

#include <rgx_types.h>
#include <rgx_log.h>
#include <rgx_alloc.h>
#include <rgx_string.h>
#include <rgx_arena.h>

typedef struct rgx_event_s rgx_event_t;
typedef struct rgx_connection_s rgx_connection_t;

typedef void (*rgx_event_handler_pt)(rgx_event_t *ev);

#include <rgx_connection.h>
#include <rgx_cycle.h>
#include <rgx_epoll.h>
#include <rgx_event.h>

#endif


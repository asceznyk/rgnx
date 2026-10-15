#ifndef RGX_CORE_H
#define RGX_CORE_H

#include <stdlib.h>
#include <sys/socket.h>

#include <rgx_types.h>
#include <rgx_log.h>
#include <rgx_alloc.h>
#include <rgx_string.h>
#include <rgx_arena.h>

typedef struct rgx_event_s rgx_event_t;
typedef struct rgx_connection_s rgx_connection_t;

typedef void (*rgx_event_handler_pt)(rgx_event_t *ev);

#include <rgx_event.h>
#include <rgx_connection.h>
#include <rgx_cycle.h>

#endif


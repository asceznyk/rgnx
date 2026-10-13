#ifndef RGX_CONNECTION_H
#define RGX_CONNECTION_H

#include <rgx_core.h>

typedef struct rgx_listening_s {
  int fd;
  struct sockaddr *sockaddr;
  socklen_t socklen;
  rgx_connection_t *connection;
} rgx_listening_t;

typedef struct rgx_connection_s {
  int fd;
  rgx_arena_t *arena;
  rgx_event_t *read;
  rgx_event_t *write;
  rgx_listening_t *listening;
  unsigned close:1;
} rgx_connection_t;

#endif



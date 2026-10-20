#ifndef RGX_CONNECTION_H
#define RGX_CONNECTION_H

#include <rgx_core.h>

typedef struct rgx_listening_s rgx_listening_t;

typedef ssize_t (*rgx_recv_pt)(rgx_connection_t *c, u_char *buf, size_t size);
typedef ssize_t (*rgx_send_pt)(rgx_connection_t *c, u_char *buf, size_t size);

struct rgx_listening_s {
  int fd;
  struct sockaddr *sockaddr;
  socklen_t socklen;
  rgx_connection_t *connection;
};

struct rgx_connection_s {
  int fd;
  rgx_arena_t *arena;
  rgx_event_t *read;
  rgx_event_t *write;
  rgx_listening_t *listening;
  rgx_recv_pt recv;
  rgx_send_pt send;
  size_t sent;
  unsigned close:1;
};

#endif



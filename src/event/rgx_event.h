#ifndef RGX_EVENT_H
#define RGX_EVENT_H

#include <rgx_core.h>

struct rgx_event_s {
  void *data;
  unsigned write:1;
  unsigned active:1;
  unsigned ready:1;
  unsigned delayed:1;
  unsigned timedout:1;
  unsigned eof:1;
  unsigned pending_eof:1;
  unsigned error:1;
  int available;
  rgx_event_handler_pt handler;
};

#endif



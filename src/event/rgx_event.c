#include <rgx_core.h>
#include <rgx_event.h>

void
rgx_event_init(rgx_cycle_t *cycle)
{
  rgx_epoll_init(cycle);
  rgx_log_info("rgx_event_init: on cycle = %p", cycle);
}

void
rgx_process_events(rgx_cycle_t *cycle)
{
  rgx_log_info("rgx_process_events: on cycle = %p", cycle);
}


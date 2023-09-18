#include "servy_internal.h"

#include <stdlib.h>

int servy_event_container_create(struct servy_event_container *container_addr,
                                 int (*callback)(struct servy *, void *, enum servy_register_event), void *userdata,
                                 enum servy_register_event on_events)
{
    container_addr->callback = callback;
    container_addr->userdata = userdata;
    container_addr->trigger_events = on_events;
    return 0;
}

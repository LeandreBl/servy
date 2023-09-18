#include "servy_internal.h"

#include <stddef.h>

int servy_event_container_trigger_with_event(struct servy_event_container *container, struct servy *servy_ctx,
                                             enum servy_register_event events)
{
    if (container->trigger_events & events) {
        if (container->callback != NULL) {
            return container->callback(servy_ctx, container->userdata, events);
        }
    }
    return 0;
}

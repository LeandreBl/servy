#include "servy_internal.h"

#include <string.h>

void servy_event_container_destroy(struct servy_event_container *container)
{
    memset(container, 0, sizeof(*container));
}

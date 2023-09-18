#include "servy_tcp_listener.h"
#include "servy_internal.h"

#include <stddef.h>
#include <string.h>

void servy_tcp_listener_destroy(struct servy_tcp_listener *listener)
{
    if (listener->servy_ctx != NULL) {
        servy_unregister_all_socky_events(listener->servy_ctx,  listener->socket.fd);
    }
    listener->servy_ctx = NULL;
    socky_destroy(&listener->socket);
    memset(&listener->params, 0, sizeof(listener->params));
    memset((void *) &listener->event_container, 0, sizeof(listener->event_container));
}
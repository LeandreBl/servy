#include "servy_tcp_listener.h"

#include <stddef.h>
#include <string.h>

int servy_tcp_listener_create(struct servy_tcp_listener *listener, const struct servy_tcp_listener_params *params)
{
    if (socky_create(&listener->socket, SOCKY_TCP) == -1) {
        return -1;
    }
    if (socky_set_options(&listener->socket, params->socket_options) == -1) {
        return -1;
    }
    if (socky_listen(&listener->socket, params->port, params->backlog) == -1) {
        return -1;
    }
    listener->params = *params;
    listener->servy_ctx = NULL;
    memset((void *) &listener->event_container, 0, sizeof(listener->event_container));
    return 0;
}

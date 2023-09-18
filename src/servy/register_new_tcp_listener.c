#include "servy.h"
#include "servy_tcp_listener.h"

#include <errno.h>

int servy_register_new_tcp_listener(struct servy *servy_ctx, const struct servy_tcp_listener_params *params)
{
    struct servy_tcp_listener *listener = NULL;
    struct servy_tcp_listener **iterator = NULL;

    iterator = lvector_find_if(servy_ctx->tcp_listeners, servy_tcp_listener_port_compare, params->port);
    if (iterator != NULL) {
        errno = EADDRINUSE;
        return -1;
    }
    listener = malloc(sizeof(*listener));
    if (listener == NULL) {
        errno = ENOMEM;
        return -1;
    }
    if (servy_tcp_listener_create(listener, params) == -1 || lvector_push_back(servy_ctx->tcp_listeners, listener) == -1) {
        free(listener);
        return -1;
    }
    if (servy_event_container_create((struct servy_event_container *)&listener->event_container, &servy_tcp_listener_event_handler,
                                     listener, SOCKY_REGISTER_EVENT_READ)
        == -1) {
        return -1;
    }
    if (servy_register_add_socky_events(servy_ctx, listener->socket.fd, &listener->event_container) == -1) {
        return -1;
    }
    listener->servy_ctx = servy_ctx;
    return 0;
}

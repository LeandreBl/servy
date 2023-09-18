#include "servy.h"
#include "servy_internal.h"

#include <errno.h>

int servy_unregister_tcp_listener(struct servy *servy_ctx, uint16_t port)
{
    struct servy_tcp_listener **iterator = lvector_find_if(servy_ctx->tcp_listeners, servy_tcp_listener_port_compare, port);

    if (iterator == NULL) {
        errno = ENOENT;
        return -1;
    }
    if (servy_unregister_all_socky_events(servy_ctx, (*iterator)->socket.fd) == -1) {
        return -1;
    }
    return lvector_erase_from_ptr(servy_ctx->tcp_listeners, iterator);
}
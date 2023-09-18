#include "servy_internal.h"
#include "servy_tcp_client.h"
#include "servy.h"

#include <errno.h>

int servy_disconnect_tcp_client(struct servy *servy_ctx, struct servy_tcp_client *client)
{
    struct servy_tcp_client **iterator;

    if (client->callbacks.on_disconnect_callback != NULL) {
        client->callbacks.on_disconnect_callback(servy_ctx, client, client->callbacks.on_disconnect_userdata, 0);
    }
    if (servy_unregister_all_socky_events(servy_ctx, client->buffered_socket.socky.fd) == -1 && errno != ENOENT) {
        return -1;
    }
    iterator = lvector_macro_find_if(servy_ctx->tcp_clients, entry, *entry == client);

    if (iterator == NULL) {
        return 0;
    }
    return lvector_erase_from_ptr(servy_ctx->tcp_clients, iterator);
}
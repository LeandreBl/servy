#include "servy_tcp_listener.h"
#include "servy_tcp_client.h"
#include "socky.h"

#include <stddef.h>
#include <errno.h>

int servy_tcp_listener_accept(struct servy_tcp_listener *listener, struct servy_tcp_client *new_uninitialized_socky_client)
{
    int error_code;

    if (socky_accept(&listener->socket, &new_uninitialized_socky_client->buffered_socket.socky) == -1) {
        return -1;
    }
    if (servy_tcp_client_create_from_accepted_socky(new_uninitialized_socky_client, &new_uninitialized_socky_client->buffered_socket.socky) == -1) {
        error_code = errno;
    } else {
        error_code = 0;
    }
    if (listener->params.callbacks.on_connect_callback != NULL) {
        listener->params.callbacks.on_connect_callback(listener->servy_ctx, listener, new_uninitialized_socky_client, listener->params.callbacks.on_connect_userdata, error_code);
    }
    return error_code == 0 ? 0 : -1;
}

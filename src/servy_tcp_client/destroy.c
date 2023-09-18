#include "servy_tcp_client.h"
#include "servy_internal.h"

#include <string.h>

void servy_tcp_client_destroy(struct servy_tcp_client *client_socket)
{
    if (client_socket->servy_ctx != NULL) {
        servy_unregister_all_socky_events(client_socket->servy_ctx, client_socket->buffered_socket.socky.fd);
    }
    memset(&client_socket->callbacks, 0, sizeof(client_socket->callbacks));
    memset((void *) &client_socket->event_container, 0, sizeof(client_socket->event_container));
    buffered_socky_destroy(&client_socket->buffered_socket);
    client_socket->servy_ctx = NULL;
}
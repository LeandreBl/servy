#include "servy_internal.h"
#include "servy_tcp_client.h"

ssize_t servy_tcp_client_write(struct servy_tcp_client *client_socket, const void *buf, size_t count)
{
    if (client_socket->buffered_socket.buffy_writer.size == 0
        && (servy_event_container_create((struct servy_event_container *)&client_socket->event_container, &servy_tcp_client_event_handler,
                                         client_socket, SOCKY_REGISTER_EVENT_WRITE | SOCKY_REGISTER_EVENT_READ | SOCKY_REGISTER_EVENT_DISCONNECT)
                == -1
            || servy_register_mod_socky_events(client_socket->servy_ctx, client_socket->buffered_socket.socky.fd,
                                               &client_socket->event_container)
                   == -1)) {
        return -1;
    }
    return buffered_socky_write(&client_socket->buffered_socket, buf, count);
}
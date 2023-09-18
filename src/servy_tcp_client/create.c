#include "servy_tcp_client.h"

#include <string.h>

int servy_tcp_client_create(struct servy_tcp_client *client_socket)
{
    const struct buffered_socky_params params = {
        .buffy_reader_allocation_strategy = BUFFY_DYNAMIC_SIZE_STRATEGY,
        .buffy_reader_capacity = SERVY_TCP_CLIENT_DEFAULT_BUFFER_SIZE,
        .buffy_writer_allocation_strategy = BUFFY_DYNAMIC_SIZE_STRATEGY,
        .buffy_writer_capacity = SERVY_TCP_CLIENT_DEFAULT_BUFFER_SIZE,
    };
    if (buffered_socky_create(&client_socket->buffered_socket, &params) == -1) {
        return -1;
    }
    client_socket->servy_ctx = NULL;
    memset(&client_socket->callbacks, 0, sizeof(client_socket->callbacks));
    memset((void *) &client_socket->event_container, 0, sizeof(client_socket->event_container));
    return 0;
}

int servy_tcp_client_create_from_accepted_socky(struct servy_tcp_client *client_socket, const struct socky *socky)
{
    const struct buffered_socky_params params = {
        .buffy_reader_allocation_strategy = BUFFY_DYNAMIC_SIZE_STRATEGY,
        .buffy_reader_capacity = SERVY_TCP_CLIENT_DEFAULT_BUFFER_SIZE,
        .buffy_writer_allocation_strategy = BUFFY_DYNAMIC_SIZE_STRATEGY,
        .buffy_writer_capacity = SERVY_TCP_CLIENT_DEFAULT_BUFFER_SIZE,
    };
    if (buffered_socky_create_from_socky(&client_socket->buffered_socket, socky, &params) == -1) {
        return -1;
    }
    client_socket->servy_ctx = NULL;
    memset(&client_socket->callbacks, 0, sizeof(client_socket->callbacks));
    memset((void *) &client_socket->event_container, 0, sizeof(client_socket->event_container));
    return 0;
}
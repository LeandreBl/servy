#include "servy_tcp_client.h"

ssize_t servy_tcp_client_peek(struct servy_tcp_client *client_socket, void *data, size_t size)
{
    return buffy_peek(&client_socket->buffered_socket.buffy_reader, data, size);
}
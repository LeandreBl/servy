#include "servy_tcp_client.h"

ssize_t servy_tcp_client_read(struct servy_tcp_client *client_socket, void *buf, size_t count)
{
    return buffy_read(&client_socket->buffered_socket.buffy_reader, buf, count);
}
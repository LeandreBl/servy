#include "servy_tcp_client.h"

ssize_t servy_tcp_client_find(struct servy_tcp_client *client_socket, const void *data_pattern, size_t buf_pattern_size)
{
    return buffy_find(&client_socket->buffered_socket.buffy_reader, data_pattern, buf_pattern_size);
}
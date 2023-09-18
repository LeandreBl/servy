#include "servy_tcp_client.h"

int servy_tcp_client_read_buffer_discard(struct servy_tcp_client *client_socket, size_t size)
{
    buffy_discard(&client_socket->buffered_socket.buffy_reader, size);
    return 0;
}

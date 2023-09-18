#include "servy_tcp_client.h"

int servy_tcp_client_read_buffer_set_allocation_strategy(struct servy_tcp_client *client_socket, enum buffy_allocation_strategy strategy)
{
    client_socket->buffered_socket.buffy_reader.flags = strategy;
    return 0;
}

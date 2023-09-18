#include "servy_tcp_client.h"

int servy_tcp_client_read_buffer_clear(struct servy_tcp_client *client_socket)
{
    buffy_clear(&client_socket->buffered_socket.buffy_reader);
    return 0;
}

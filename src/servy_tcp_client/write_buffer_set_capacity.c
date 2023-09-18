#include <errno.h>

#include "servy_tcp_client.h"

int servy_tcp_client_write_buffer_set_capacity(struct servy_tcp_client *client_socket, size_t size)
{
    if (size < client_socket->buffered_socket.buffy_writer.size) {
        errno = EINVAL;
        return -1;
    }
    return buffy_resize_capacity(&client_socket->buffered_socket.buffy_writer, size);
}

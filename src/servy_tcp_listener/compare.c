#include "servy_tcp_listener.h"

int servy_tcp_listener_port_compare(struct servy_tcp_listener **plistener, uint16_t port)
{
    uint16_t listener_port;

    if (socky_get_port(&(*plistener)->socket, &listener_port) == -1) {
        return -1;
    }
    return port - listener_port;
}
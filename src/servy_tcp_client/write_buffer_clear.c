#include "buffered_socky.h"
#include "servy.h"
#include "servy_internal.h"
#include "servy_tcp_client.h"

int servy_tcp_client_write_buffer_clear(struct servy_tcp_client *client_socket)
{
    struct servy_event_container event_container = {
        .callback = &servy_tcp_client_event_handler,
        .trigger_events = SOCKY_REGISTER_EVENT_READ | SOCKY_REGISTER_EVENT_DISCONNECT,
        .userdata = client_socket,
    };
    buffy_clear(&client_socket->buffered_socket.buffy_writer);
    return servy_register_mod_socky_events(client_socket->servy_ctx, client_socket->buffered_socket.socky.fd, &event_container);
}
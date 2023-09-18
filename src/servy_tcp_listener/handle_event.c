#include "servy.h"
#include "servy_tcp_client.h"
#include "servy_tcp_listener.h"

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

static int servy_tcp_listener_emplace_client(struct servy_tcp_client **tcp_client, struct servy_tcp_listener *tcp_listener,
                                             struct servy *servy_ctx)
{
    int socket_flags;

    *tcp_client = malloc(sizeof(**tcp_client));
    if (*tcp_client == NULL) {
        errno = ENOMEM;
        return -1;
    }
    if (servy_tcp_listener_accept(tcp_listener, *tcp_client) == -1) {
        return -1;
    }
    ((struct servy_event_container *)&(*tcp_client)->event_container)->trigger_events
        = SOCKY_REGISTER_EVENT_READ | SOCKY_REGISTER_EVENT_DISCONNECT;
    ((struct servy_event_container *)&(*tcp_client)->event_container)->userdata = *tcp_client;
    ((struct servy_event_container *)&(*tcp_client)->event_container)->callback = &servy_tcp_client_event_handler;
    if (servy_register_add_socky_events(servy_ctx, (*tcp_client)->buffered_socket.socky.fd, &(*tcp_client)->event_container) == -1) {
        return -1;
    }
    socket_flags = fcntl((*tcp_client)->buffered_socket.socky.fd, F_GETFL, 0);
    if (socket_flags == -1) {
        return -1;
    }
    if (fcntl((*tcp_client)->buffered_socket.socky.fd, F_SETFL, socket_flags | O_NONBLOCK) == -1) {
        return -1;
    }
    (*tcp_client)->servy_ctx = servy_ctx;
    return 0;
}

static int servy_tcp_listener_async_accept_client(struct servy *servy_ctx, struct servy_tcp_listener *tcp_listener)
{
    return lvector_emplace_back(servy_ctx->tcp_clients, servy_tcp_listener_emplace_client, tcp_listener, servy_ctx);
}

int servy_tcp_listener_event_handler(struct servy *servy_ctx, void *userdata, enum servy_register_event event)
{
    struct servy_tcp_listener *tcp_listener = userdata;

    if (event & SOCKY_REGISTER_EVENT_READ) {
        return dispatchy_enqueue(&servy_ctx->dispatcher, &servy_tcp_listener_async_accept_client, servy_ctx, tcp_listener);
    }
    return 0;
}
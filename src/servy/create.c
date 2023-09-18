
#include "servy.h"
#include "servy_tcp_client.h"
#include "servy_tcp_listener.h"

#include <sys/epoll.h>

static void servy_tcp_listener_destructor(struct servy_tcp_listener **pointer)
{
    servy_tcp_listener_destroy(*pointer);
    free(*pointer);
}

static void servy_tcp_client_destructor(struct servy_tcp_client **pointer)
{
    socky_shutdown(&(*pointer)->buffered_socket.socky, SOCKY_SHUTDOWN_READ_WRITE);
    servy_tcp_client_destroy(*pointer);
    free(*pointer);
}

int servy_create(struct servy *servy_ctx)
{
    if (lvector_create(servy_ctx->tcp_listeners, 0, servy_tcp_listener_destructor) == -1) {
        return -1;
    }
    if (lvector_create(servy_ctx->tcp_clients, 0, servy_tcp_client_destructor) == -1) {
        return -1;
    }
    if (dispatchy_create(&servy_ctx->dispatcher) == -1) {
        return -1;
    }
    servy_ctx->servy_epoll_fd = epoll_create1(0);
    if (servy_ctx->servy_epoll_fd == -1) {
        return -1;
    }
    servy_ctx->running = false;
    return 0;
}
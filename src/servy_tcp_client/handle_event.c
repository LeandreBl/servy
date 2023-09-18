#include "buffered_socky.h"
#include "servy.h"
#include "servy_tcp_client.h"

#include <errno.h>

static int servy_tcp_client_async_read_from_client(struct servy_tcp_client *tcp_client, struct servy *servy_ctx)
{
    ssize_t rd = buffered_socky_read(&tcp_client->buffered_socket, SERVY_TCP_CLIENT_DEFAULT_BUFFER_SIZE);

    if (rd == 0) {
        return servy_disconnect_tcp_client(servy_ctx, tcp_client);
    }
    if (rd < 0) {
        return -1;
    }
    if (tcp_client->callbacks.after_read_callback != NULL) {
        return dispatchy_enqueue(&servy_ctx->dispatcher, tcp_client->callbacks.after_read_callback, servy_ctx, tcp_client,
                                 tcp_client->callbacks.after_read_userdata, 0);
    }
    return 0;
}

static int servy_tcp_client_async_write_to_client(struct servy_tcp_client *tcp_client, struct servy *servy_ctx)
{
    ssize_t wr;

    if (tcp_client->callbacks.before_write_callback != NULL) {
        if (dispatchy_enqueue(&servy_ctx->dispatcher, tcp_client->callbacks.before_write_callback, servy_ctx, tcp_client,
                              tcp_client->callbacks.before_write_userdata, 0)
            == -1) {
            return -1;
        }
    }
    wr = buffered_socky_flush(&tcp_client->buffered_socket);
    if (wr < 0) {
        return -1;
    }
    if (tcp_client->buffered_socket.buffy_writer.size == 0
        && (servy_event_container_create((struct servy_event_container *)&tcp_client->event_container, &servy_tcp_client_event_handler,
                                         tcp_client, SOCKY_REGISTER_EVENT_READ | SOCKY_REGISTER_EVENT_DISCONNECT)
                == -1
            || servy_register_mod_socky_events(tcp_client->servy_ctx, tcp_client->buffered_socket.socky.fd, &tcp_client->event_container)
                   == -1)) {
        return -1;
    }
    return 0;
}

static int servy_tcp_client_async_disconnect(struct servy_tcp_client *tcp_client, struct servy *servy_ctx)
{
    return servy_disconnect_tcp_client(servy_ctx, tcp_client);
}

int servy_tcp_client_event_handler(struct servy *servy_ctx, void *userdata, enum servy_register_event event)
{
    struct servy_tcp_client *tcp_client = userdata;

    if (event & SOCKY_REGISTER_EVENT_DISCONNECT) {
        return dispatchy_enqueue(&servy_ctx->dispatcher, &servy_tcp_client_async_disconnect, tcp_client, servy_ctx);
    }
    if (event & SOCKY_REGISTER_EVENT_READ) {
        if (dispatchy_enqueue(&servy_ctx->dispatcher, &servy_tcp_client_async_read_from_client, tcp_client, servy_ctx) == -1) {
            return -1;
        }
    }
    if (event & SOCKY_REGISTER_EVENT_WRITE) {
        if (dispatchy_enqueue(&servy_ctx->dispatcher, &servy_tcp_client_async_write_to_client, tcp_client, servy_ctx) == -1) {
            return -1;
        }
    }
    return 0;
}
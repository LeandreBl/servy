#ifndef SERVY_TCP_LISTENER_H_
#define SERVY_TCP_LISTENER_H_

#include <stdint.h>

#include <socky.h>
#include <servy_internal.h>

/**
 * Forward declarations
 */
struct servy;
struct servy_tcp_client;
struct servy_tcp_listener;

struct servy_listener_callbacks
{
    void (*on_connect_callback)(struct servy *servy_ctx, struct servy_tcp_listener *servy_listener, struct servy_tcp_client *servy_socket, void *userdata, int errno_code);
    void *on_connect_userdata;
};

struct servy_tcp_listener_params {
    uint16_t port;
    uint32_t backlog;
    int socket_options;
    struct servy_listener_callbacks callbacks;
};

struct servy_tcp_listener
{
    struct socky socket;
    struct servy_tcp_listener_params params;
    struct servy *servy_ctx;
    const struct servy_event_container event_container;
};

int servy_tcp_listener_create(struct servy_tcp_listener *listener, const struct servy_tcp_listener_params *params) __nonnull((1, 2));

int servy_tcp_listener_accept(struct servy_tcp_listener *listener, struct servy_tcp_client *new_uninitialized_socky_client) __nonnull((1, 2));

void servy_tcp_listener_destroy(struct servy_tcp_listener *listener) __nonnull((1));

int servy_tcp_listener_port_compare(struct servy_tcp_listener **plistener, uint16_t port) __nonnull((1));

#endif /* !SERVY_TCP_LISTENER_H_ */
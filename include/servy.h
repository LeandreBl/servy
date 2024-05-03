#ifndef SERVY_H_
#define SERVY_H_

#include <stdint.h>
#include <stdbool.h>

#include <lvector.h>
#include <socky.h>
#include <servy_internal.h>
#include <servy_tcp_listener.h>
#include <dispatchy.h>

struct servy
{
    lvector(struct servy_tcp_listener *) tcp_listeners;
    lvector(struct servy_tcp_client *) tcp_clients;
    struct dispatchy dispatcher;
    int servy_epoll_fd;
    bool running : 1;
};

int servy_create(struct servy *servy_ctx) __nonnull((1));

void servy_destroy(struct servy *servy_ctx) __nonnull((1));

int servy_register_new_tcp_listener(struct servy *servy_ctx, const struct servy_tcp_listener_params *params) __nonnull((1, 2));

int servy_unregister_tcp_listener(struct servy *servy_ctx, uint16_t port) __nonnull((1));

int servy_disconnect_tcp_client(struct servy *servy_ctx, struct servy_tcp_client *client) __nonnull((1, 2));

int servy_timeout_poll(struct servy *servy_ctx, int32_t timeout_ms)  __nonnull((1));

int servy_tcp_connect(struct servy *servy_ctx, const struct servy_duplex_callbacks *callbacks, const char *address, uint16_t port);

int servy_tcp_connect32(struct servy *servy_ctx, const struct servy_duplex_callbacks *callbacks, uint32_t address, uint16_t port);

int servy_run(struct servy *servy_ctx) __nonnull((1));

#endif /* !SERVY_H_ */
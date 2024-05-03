#ifndef servy_tcp_client_H_
#define servy_tcp_client_H_

#include <buffered_socky.h>
#include <servy_internal.h>

#define SERVY_TCP_CLIENT_DEFAULT_BUFFER_SIZE (2048)

/* Forward declaration */
struct servy;
struct servy_tcp_client;

struct servy_duplex_callbacks
{
    void (*on_connect_callback)(struct servy *servy_ctx, struct servy_tcp_client *servy_socket, void *userdata, int errno_code);
    void *on_connect_userdata;
    void (*on_disconnect_callback)(struct servy *servy_ctx, struct servy_tcp_client *servy_socket, void *userdata, int errno_code);
    void *on_disconnect_userdata;
    void (*after_read_callback)(struct servy *servy_ctx, struct servy_tcp_client *servy_socket, void *userdata, int errno_code);
    void *after_read_userdata;
    void (*before_write_callback)(struct servy *servy_ctx, struct servy_tcp_client *servy_socket, void *userdata, int errno_code);
    void *before_write_userdata;
};

struct servy_tcp_client
{
    struct buffered_socky buffered_socket;
    struct servy_duplex_callbacks callbacks;
    struct servy *servy_ctx;
    const struct servy_event_container event_container;
};

int servy_tcp_client_create(struct servy_tcp_client *client_socket) __nonnull((1));
int servy_tcp_client_create_from_accepted_socky(struct servy_tcp_client *client_socket, const struct socky *socky) __nonnull((1, 2));
void servy_tcp_client_destroy(struct servy_tcp_client *client_socket) __nonnull((1));
ssize_t servy_tcp_client_write(struct servy_tcp_client *client_socket, const void *buf, size_t count) __nonnull((1, 2));
ssize_t servy_tcp_client_read(struct servy_tcp_client *client_socket, void *buf, size_t count) __nonnull((1, 2));
ssize_t servy_tcp_client_peek(struct servy_tcp_client *client_socket, void *buf, size_t count) __nonnull((1, 2));
ssize_t servy_tcp_client_find(struct servy_tcp_client *client_socket, const void *data_pattern, size_t size) __nonnull((1, 2));
int servy_tcp_client_read_buffer_set_capacity(struct servy_tcp_client *client_socket, size_t size) __nonnull((1));
int servy_tcp_client_read_buffer_set_allocation_strategy(struct servy_tcp_client *client_socket, enum buffy_allocation_strategy strategy) __nonnull((1));
int servy_tcp_client_read_buffer_discard(struct servy_tcp_client *client_socket, size_t size) __nonnull((1));
int servy_tcp_client_read_buffer_clear(struct servy_tcp_client *client_socket) __nonnull((1));
int servy_tcp_client_write_buffer_set_capacity(struct servy_tcp_client *client_socket, size_t size) __nonnull((1));
int servy_tcp_client_write_buffer_set_allocation_strategy(struct servy_tcp_client *client_socket, enum buffy_allocation_strategy strategy) __nonnull((1));
int servy_tcp_client_write_buffer_discard(struct servy_tcp_client *client_socket, size_t size) __nonnull((1));
int servy_tcp_client_write_buffer_clear(struct servy_tcp_client *client_socket) __nonnull((1));

#endif /* !servy_tcp_client_H_ */

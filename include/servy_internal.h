#ifndef SERVY_INTERNAL_H_
#define SERVY_INTERNAL_H_

#include <sys/epoll.h>

/**
* Forward declarations
*/
struct servy;

enum servy_register_event {
    SOCKY_REGISTER_EVENT_READ = EPOLLIN,
    SOCKY_REGISTER_EVENT_WRITE = EPOLLOUT,
    SOCKY_REGISTER_EVENT_DISCONNECT = EPOLLRDHUP,
};

struct servy_event_container {
    int (* callback)(struct servy *servy_ctx, void *userdata, enum servy_register_event event);
    void *userdata;
    enum servy_register_event trigger_events;
};

int servy_event_container_create(struct servy_event_container *container_addr, int (* callback)(struct servy *, void *, enum servy_register_event), void *userdata, enum servy_register_event on_events) __nonnull((1, 2));
void servy_event_container_destroy(struct servy_event_container *container) __nonnull((1));
int servy_event_container_trigger_with_event(struct servy_event_container *container, struct servy *servy_ctx, enum servy_register_event events) __nonnull((1));

int servy_register_add_socky_events(struct servy *servy_ctx, int fd, const struct servy_event_container *event_container) __nonnull((1, 3));
int servy_register_mod_socky_events(struct servy *servy_ctx, int fd, const struct servy_event_container *event_container) __nonnull((1, 3));
int servy_unregister_all_socky_events(struct servy *servy_ctx, int fd);

int servy_tcp_client_event_handler(struct servy *servy_ctx, void *userdata, enum servy_register_event event);
int servy_tcp_listener_event_handler(struct servy *servy_ctx, void *userdata, enum servy_register_event event);


#endif /* !SERVY_INTERNAL_H_ */
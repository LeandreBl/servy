#include "servy.h"
#include "servy_internal.h"

#include <stddef.h>

int servy_register_add_socky_events(struct servy *servy_ctx, int fd, const struct servy_event_container *event_container)
{
    struct epoll_event epoll_events = {
        .data.ptr = (void *) event_container,
        .events = event_container->trigger_events,
    };

    if (epoll_ctl(servy_ctx->servy_epoll_fd, EPOLL_CTL_ADD, fd, &epoll_events) == -1) {
        return -1;
    }
    return 0;
}

int servy_register_mod_socky_events(struct servy *servy_ctx, int fd, const struct servy_event_container *event_container)
{
    struct epoll_event epoll_events = {
        .data.ptr = (void *) event_container,
        .events = event_container->trigger_events,
    };

    if (epoll_ctl(servy_ctx->servy_epoll_fd, EPOLL_CTL_MOD, fd, &epoll_events) == -1) {
        return -1;
    }
    return 0;
}

int servy_unregister_all_socky_events(struct servy *servy_ctx, int fd)
{
    if (epoll_ctl(servy_ctx->servy_epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        return -1;
    }
    return 0;
}

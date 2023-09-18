#include "servy.h"

#include <sys/epoll.h>

int servy_timeout_poll(struct servy *servy_ctx, int32_t timeout_ms)
{
    lvector(struct epoll_event) events;
    size_t total_possible_events = servy_ctx->tcp_listeners.len + servy_ctx->tcp_clients.len;
    struct servy_event_container *event_container;
    int event_count;

    if (lvector_create(events, total_possible_events, NULL) == -1) {
        return -1;
    }
    event_count = epoll_wait(servy_ctx->servy_epoll_fd, events.arr, events.rsize, timeout_ms);
    if (event_count < 0) {
        lvector_destroy(events);
        return -1;
    }
    events.len = event_count;
    lvector_foreach(event, events)
    {
        event_container = event->data.ptr;
        if (dispatchy_enqueue(&servy_ctx->dispatcher, &servy_event_container_trigger_with_event, event_container, servy_ctx, event->events)
            == -1) {
            lvector_destroy(events);
            return -1;
        }
    }
    if (dispatchy_process(&servy_ctx->dispatcher, 0, timeout_ms) == -1) {
        lvector_destroy(events);
        return -1;
    }
    lvector_destroy(events);
    return 0;
}
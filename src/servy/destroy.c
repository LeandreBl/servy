#include "servy.h"
#include "servy_internal.h"

#include <sys/epoll.h>
#include <unistd.h>

void servy_destroy(struct servy *servy_ctx)
{
    lvector_destroy(servy_ctx->tcp_listeners);
    lvector_destroy(servy_ctx->tcp_clients);
    dispatchy_destroy(&servy_ctx->dispatcher);
    close(servy_ctx->servy_epoll_fd);
    servy_ctx->servy_epoll_fd = -1;
    servy_ctx->running = false;
}
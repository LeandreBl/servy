#include "servy.h"

int servy_run(struct servy *servy_ctx)
{
    servy_ctx->running = true;

    while (servy_ctx->running == true) {
        if (servy_timeout_poll(servy_ctx, -1) == -1) {
            return -1;
        }
    }
    return 0;
}
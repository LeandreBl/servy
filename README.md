# servy
A little TCP asynchronous linux server in C

This library is based on my other libraries:
- [lvector](https://github.com/LeandreBl/lvector)
- [socky](https://github.com/LeandreBl/socky)
- [buffy](https://github.com/LeandreBl/buffy)
- [buffered-socky](https://github.com/LeandreBl/buffered-socky)
- [dispatchy](https://github.com/LeandreBl/dispatchy)

# Build

```sh
make
```

# Tests

```
make tests_run
```

# Install

```sh
sudo make install
```

# Clean

```
make clean
```

# Example of a quick echo server

```c
#include <servy.h>
#include <servy_tcp_client.h>
#include <stdio.h>

static void async_after_read(struct servy *servy_ctx, struct servy_tcp_client *servy_socket, void *userdata, int errno_code)
{
    const char *title = userdata;
    char *addr;
    uint16_t port;

    /**
    * Get the ipv4 address string of the socket
    * This function can fail, but we don't care for this example
    */
    socky_get_address_str(&servy_socket->buffered_socket.socky, &addr);

    /**
    * Get the ipv4 port of the socket
    * This function can fail, but we don't care for this example
    */
    socky_get_port(&servy_socket->buffered_socket.socky, &port);

    printf("%s: %s:%u read %zu bytes: [%.*s]\n", title, addr, port, servy_socket->buffered_socket.buffy_reader.size,
           (int)servy_socket->buffered_socket.buffy_reader.size, (char *)servy_socket->buffered_socket.buffy_reader.data);

    /**
    * Write back the read buffer to the client
    * This function can fail, but we don't care for this example
    */
    servy_tcp_client_write(servy_socket, servy_socket->buffered_socket.buffy_reader.data, servy_socket->buffered_socket.buffy_reader.size);

    /**
    * We "consumed" the data without using the "buffy_read" function that discards the data it reads from the buffy structure
    * So we need to manually clear it
    */
    buffy_clear(&servy_socket->buffered_socket.buffy_reader);

    /**
    * The addr returned by socky_get_address_str needs to be freed
    */
    free(addr);
}

static void async_before_write(struct servy *servy_ctx, struct servy_tcp_client *servy_socket, void *userdata, int errno_code)
{
    const char *title = userdata;
    char *addr;
    uint16_t port;

    /**
    * Get the ipv4 address string of the socket
    * This function can fail, but we don't care for this example
    */
    socky_get_address_str(&servy_socket->buffered_socket.socky, &addr);

    /**
    * Get the ipv4 port of the socket
    * This function can fail, but we don't care for this example
    */
    socky_get_port(&servy_socket->buffered_socket.socky, &port);

    printf("%s: %s:%u write %zu bytes: [%.*s]\n", title, addr, port, servy_socket->buffered_socket.buffy_writer.size,
           (int)servy_socket->buffered_socket.buffy_writer.size, (char *)servy_socket->buffered_socket.buffy_writer.data);

    /**
    * The addr returned by socky_get_address_str needs to be freed
    */
    free(addr);
}

static void async_on_disconnect(struct servy *servy_ctx, struct servy_tcp_client *servy_socket, void *userdata, int errno_code)
{
    const char *title = userdata;
    char *addr;
    uint16_t port;

    /**
    * Get the ipv4 address string of the socket
    * This function can fail, but we don't care for this example
    */
    socky_get_address_str(&servy_socket->buffered_socket.socky, &addr);

    /**
    * Get the ipv4 port of the socket
    * This function can fail, but we don't care for this example
    */
    socky_get_port(&servy_socket->buffered_socket.socky, &port);

    printf("%s: %s:%u disconnected\n", title, addr, port);

    /**
    * The addr returned by socky_get_address_str needs to be freed
    */
    free(addr);

    /**
    * Stop the server when one of the clients disconnects
    * It's just for the example, I'm pretty sure no one needs to do that
    */
    servy_ctx->running = false;
}

static void async_on_connect(struct servy *servy_ctx, struct servy_tcp_listener *servy_listener, struct servy_tcp_client *servy_socket,
                             void *userdata, int errno_code)
{
    const char *title = userdata;
    char *addr;
    uint16_t port;
    uint16_t listener_port;

    /**
    * Get the ipv4 address string of the socket
    * This function can fail, but we don't care for this example
    */
    socky_get_address_str(&servy_socket->buffered_socket.socky, &addr);

    /**
    * Get the ipv4 port of the socket
    * This function can fail, but we don't care for this example
    */
    socky_get_port(&servy_socket->buffered_socket.socky, &port);

    /**
    * Get the ipv4 port of the listener
    * This function can fail, but we don't care for this example
    */
    socky_get_port(&servy_listener->socket, &listener_port);

    printf("%s: %s:%u connected from port %u\n", title, addr, port, listener_port);

    /**
    * Setup the async callbacks for each possible events
    */
    servy_socket->callbacks.after_read_callback = &async_after_read;
    servy_socket->callbacks.after_read_userdata = userdata;
    servy_socket->callbacks.before_write_callback = &async_before_write;
    servy_socket->callbacks.before_write_userdata = userdata;
    servy_socket->callbacks.on_disconnect_callback = &async_on_disconnect;
    servy_socket->callbacks.on_disconnect_userdata = userdata;

    /**
    * The addr returned by socky_get_address_str needs to be freed
    */
    free(addr);
}

int main(int ac, char **av)
{
    /**
    * Create a servy structure
    * It will contain all the data needed to run the server
    */
    struct servy ctx;

    /**
    * Create a tcp listener params structure
    * It will contain basic information to listen on a new port
    * This structure is just a container structure
    */
    struct servy_tcp_listener_params params = {
        /**
        * Setup the listener callbacks
        */
        .callbacks = {
            .on_connect_callback = &async_on_connect,
            .on_connect_userdata = "SERVY",
        },
        /**
        * Setup the backlog (number of clients that the kernel will store until "accept" calls are done before replying with a "connection refused" event)
        */
        .backlog = 10,
        /**
        * Setup the socket options for this listener (see man setsockopt)
        * Here we tag the port with REUSEADDR to avoid "address already in use" when starting the server too fast after killing it
        */
        .socket_options = SO_REUSEADDR,
    };

    /**
    * The server accepts opening multiple ports in parameter
    * Note that, even if this example does not show it, you can unregister a port at any moment
    */
    if (ac < 2) {
        fprintf(stderr, "USAGE: %s port1 [port2] [port3] ...\n", av[0]);
        return 1;
    }

    /**
    * Create a servy server
    */
    if (servy_create(&ctx) == -1) {
        fprintf(stderr, "Error in servy_create: %m\n");
        return 1;
    }

    /**
    * Register each port given as program arguments
    */
    for (int i = 1; i < ac; ++i) {

        /**
        * Huge error checking as you can obviously see
        * Note that, if the port is 0, servy_register_new_tcp_listener will bind a random available port (see man bind)
        */
        params.port = atoi(av[i]);

        /**
        * Register a new listener based on it's port, callbacks and options
        */
        if (servy_register_new_tcp_listener(&ctx, &params) == -1) {
            fprintf(stderr, "Error in servy_register_new_tcp_listener: %m\n");
            servy_destroy(&ctx);
            return 1;
        }
    }

    /**
    * Start the infinite loop of events handling
    * Socket events are poped and will fill a queue of callbacks
    * Callbacks are then poped in pushed order
    * Note that you can fill the dispatcher with any function you need
    */
    if (servy_run(&ctx) == -1) {
        fprintf(stderr, "Error in servy_run: %m\n");
        servy_destroy(&ctx);
        return 1;
    }

    /**
    * Destroy the server, disconnecting the sockets and freeing the buffers
    */
    servy_destroy(&ctx);
    return 0;
}
```

# Compilation

```sh
gcc main.c -lservy -lbuffy -lsocky -lbuffered-socky -ldispatchy -lavcall
```

> You can compile the library staticly with `make static`
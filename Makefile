NAME		= libservy.so

CC			= gcc

ARCHIVER	= ar

SRCS		= src/servy/create.c
SRCS		+= src/servy/destroy.c
SRCS		+= src/servy/register_new_tcp_listener.c
SRCS		+= src/servy/register_socky_events.c
SRCS		+= src/servy/timeout_poll.c
SRCS		+= src/servy/unregister_tcp_listener.c
SRCS		+= src/servy/disconnect_tcp_client.c
SRCS		+= src/servy/run.c

SRCS		+= src/servy_tcp_client/create.c
SRCS		+= src/servy_tcp_client/destroy.c
SRCS		+= src/servy_tcp_client/find.c
SRCS		+= src/servy_tcp_client/handle_event.c
SRCS		+= src/servy_tcp_client/peek.c
SRCS		+= src/servy_tcp_client/read_buffer_clear.c
SRCS		+= src/servy_tcp_client/read_buffer_discard.c
SRCS		+= src/servy_tcp_client/read_buffer_set_allocation_strategy.c
SRCS		+= src/servy_tcp_client/read_buffer_set_capacity.c
SRCS		+= src/servy_tcp_client/read.c
SRCS		+= src/servy_tcp_client/write_buffer_clear.c
SRCS		+= src/servy_tcp_client/write_buffer_discard.c
SRCS		+= src/servy_tcp_client/write_buffer_set_allocation_strategy.c
SRCS		+= src/servy_tcp_client/write_buffer_set_capacity.c
SRCS		+= src/servy_tcp_client/write.c

SRCS		+= src/servy_tcp_listener/create.c
SRCS		+= src/servy_tcp_listener/destroy.c
SRCS		+= src/servy_tcp_listener/compare.c
SRCS		+= src/servy_tcp_listener/handle_event.c
SRCS		+= src/servy_tcp_listener/accept.c

SRCS		+= src/servy_event_container/create.c
SRCS		+= src/servy_event_container/destroy.c
SRCS		+= src/servy_event_container/try_trigger_with_event.c

TESTS_SRCS	:= $(SRCS)

OBJS		= $(SRCS:.c=.o)
TESTS_OBJS	= $(TESTS_SRCS:.c=.o)

RM			= rm -f

DEPENDENCIES = buffy socky dispatchy buffered-socky avcall

RECURSIVE_HEADERS = $(shell dirname `find . -name "*.h"`)

CFLAGS		+= -Wall -Wextra -fPIC -pedantic
CPPFLAGS	+= -I ./include $(addprefix -iquote,$(RECURSIVE_HEADERS))
LIBS		+= $(addprefix -l,$(DEPENDENCIES))
LDFLAGS		= -shared $(LIBS) $(addprefix -L,$(DEPENDENCIES))

GREEN=`tput setaf 2`
RED=`tput setaf 1`
YELLOW=`tput setaf 3`
NO_COLOR=`tput sgr0`

%.o : %.c
	@$ $(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	@echo "$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ ["$(GREEN)"OK"$(NO_COLOR)"]"
.SUFFIXES: .o .c

all: dependencies $(NAME)

$(NAME): $(OBJS)
	@$ $(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
	@echo "$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@ \
	["$(GREEN)"LINKING OK"$(NO_COLOR)"]"

dependencies:
	git submodule update --init --recursive
	git submodule foreach $(MAKE)

tests_run: $(TESTS_OBJS)
	git submodule foreach $(MAKE) $@
	@echo $(TESTS_OBJS)
	@$ $(CC) -lcriterion $(TESTS_OBJS) $(LIBS) -o $@
	@echo "$(CC) -lcriterion $(TESTS_OBJS) $(LIBS) -o $@ \
	["$(GREEN)"LINKING OK"$(NO_COLOR)"]"
	./$@
	@pkill nc || true
	@pkill $@ || true
	@$(RM) $@
	@$(RM) $(TESTS_OBJS)

val_run: CFLAGS += -ggdb3
val_run: $(TESTS_OBJS)
	@$ $(CC) -lcriterion $(TESTS_OBJS) $(LIBS) -o $@
	@echo "$(CC) -lcriterion $(TESTS_OBJS) $(LIBS) -o $@ \
	["$(GREEN)"LINKING OK"$(NO_COLOR)"]"
	valgrind --trace-children=yes --quiet ./$@
	@pkill nc || true
	@pkill $@ || true
	@$(RM) $@
	@$(RM) $(TESTS_OBJS)

debug: CFLAGS += -ggdb3
debug: all
	git submodule foreach $(MAKE) $@ || true

clean:
	git submodule foreach $(MAKE) $@
	$(RM) $(OBJS)

fclean: clean
	git submodule foreach $(MAKE) $@
	$(RM) $(NAME) $(NAME:.so=.a)

re: fclean all

install: all
	git submodule foreach $(MAKE) $@
	@cp $(NAME) /usr/lib/$(NAME) 2> /dev/null || \
	printf "\033[1m\033[31mError : try sudo make install\033[0m\n" && \
	cp include/*.h /usr/include/ 2> /dev/null && \
	printf "\033[1m\033[32mLibrary $(NAME) successfully installed !\033[0m\n"

static: $(OBJS)
	$(ARCHIVER) rc $(NAME:.so=.a) $(OBJS)

.PHONY: all clean fclean re tests_run val_run debug install static
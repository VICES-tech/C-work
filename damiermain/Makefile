CC      = gcc
CFLAGS  = -Wall -Wextra -ansi -pedantic -std=c11 -Iinclude
LDFLAGS = -Llib -lgraph -lX11 -lm -lXpm

SRCS = src/main.c src/config.c
OBJS = $(SRCS:.c=.o)
BIN  = taquin

all: $(BIN)

		src/%.o: 	src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(BIN)

clean:
	rm -f $(OBJS) $(BIN)

.PHONY: all clean
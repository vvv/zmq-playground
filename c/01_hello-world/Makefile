LDLIBS = -lzmq
CFLAGS = -g -Wall -Wextra

all: server client

server: server.c

client: client.c

.PHONY: clean
clean:
	rm -f server client
	rm -rf server.dSYM client.dSYM

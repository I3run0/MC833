CC = gcc
CFLAGS = -Wall -Werror

all: client server

client: src/client/client.c 
	$(CC) $(CFLAGS) -o bin/client src/client/client.c

server: src/server/server.c
	$(CC) $(CFLAGS) -o bin/server src/server/server.c

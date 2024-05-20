CC = gcc
CFLAGS = -Wall -Werror
LIBS = -lsqlite3

CLIENT_SRC = src/client/client.c libs/*
SERVER_SRC = src/server/server.c src/server/database.c libs/*

BIN_DIR = bin
CLIENT_BIN = $(BIN_DIR)/client
SERVER_BIN = $(BIN_DIR)/server

.PHONY: all clean

all: $(CLIENT_BIN) $(SERVER_BIN)

$(CLIENT_BIN): $(CLIENT_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(SERVER_BIN): $(SERVER_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)


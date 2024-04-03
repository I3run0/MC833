#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFERLEN 1024
#define ENDTOKEN "\r\r"

struct message {
    char message[BUFFERLEN];
    int len;
};


struct message *create_message_w();

int recv_message_w(int sockfd, struct message *msg);

void send_message_w(int sockfd, struct message *msg);

int free_messge_w(int sockfd, struct message *msg);
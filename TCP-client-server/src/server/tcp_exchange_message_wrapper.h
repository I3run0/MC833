#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFERLEN 4016
#define ENDTOKEN "\r\r"

struct message {
    char * message;
    int len;
};


int init_message_w(struct message * msg);

int recv_message_w(int sockfd, struct message *msg);

void send_message_w(int sockfd, struct message *msg);

int free_messge_w(int sockfd, struct message *msg);
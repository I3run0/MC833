#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SIZEBUFFER 200

struct  message
{
    char *message;
    int len;
};

struct message * create_message_w();

int recv_message_w(int sockfd, struct message *msg);

void send_message_w(int sockfd, struct message *msg);

int free_message_w(struct message *msg);
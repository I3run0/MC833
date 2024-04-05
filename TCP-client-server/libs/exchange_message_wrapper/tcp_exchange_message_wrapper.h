#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SIZEBUFFER 200

char *recv_message_w(int sockfd);

void send_message_w(int sockfd, char *msg, int len);
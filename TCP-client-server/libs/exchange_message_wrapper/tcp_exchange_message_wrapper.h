#ifndef TCP_EXCHANGE_MESSAGE_WRAPPER_H
#define TCP_EXCHANGE_MESSAGE_WRAPPER_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZEBUFFER 200

char *recv_message_w(int sockfd);
void send_message_w(int sockfd, const char *msg, int len);

#endif /* TCP_EXCHANGE_MESSAGE_WRAPPER_H */

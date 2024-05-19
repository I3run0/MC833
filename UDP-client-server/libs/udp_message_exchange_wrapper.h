#ifndef UDP_MESSAGE_EXCHANGE_WRAPPER_H
#define UDP_MESSAGE_EXCHANGE_WRAPPER_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

#define TIMEOUT_SEC 5
#define BUFFERSIZE 200
 
char *recv_message_w(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
void send_message_w(int sockfd, const char *msg, int len, struct sockaddr *addr, socklen_t addrlen);

#endif /* UDP_MESSAGE_EXCHANGE_WRAPPER_H */

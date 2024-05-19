#ifndef UDP_FILE_EXCHANGE_WRAPPER_H
#define UDP_FILE_EXCHANGE_WRAPPER_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/time.h>

#define TIMEOUT_SEC 5
#define PACKETSIZE 512

void recv_file_w(int sockfd, const char *path, struct sockaddr *addr, socklen_t addrlen);
void send_file_w(int sockfd, const char *path, struct sockaddr *addr, socklen_t addrlen);

#endif /* UDP_FILE_EXCHANGE_WRAPPER_H */

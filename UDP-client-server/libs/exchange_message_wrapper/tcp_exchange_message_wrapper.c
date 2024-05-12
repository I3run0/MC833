#include "tcp_exchange_message_wrapper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int _recv_message_w(int sockfd, char *buffer, int len, struct sockaddr *addr, socklen_t *addrlen) {
    ssize_t r = 0;
    ssize_t nrecv = 0;

    while (nrecv < len) {
        r = recvfrom(sockfd, buffer + nrecv, len - nrecv, MSG_WAITALL, addr, addrlen);
        if (r <= 0) {
            return r;
        }
        nrecv += r;
    }

    buffer[nrecv] = '\0'; // Ensure null termination
    return nrecv;
}

char *recv_message_w(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int msg_len_torecv_i;
    char msg_len_torecv_c[SIZEBUFFER];

    if (_recv_message_w(sockfd, msg_len_torecv_c, SIZEBUFFER, addr, addrlen) <= 0) {
        return NULL;
    }
    msg_len_torecv_i = atoi(msg_len_torecv_c);

    char *msg = (char *)malloc((msg_len_torecv_i + 1) * sizeof(char));
    if (_recv_message_w(sockfd, msg, msg_len_torecv_i, addr, addrlen) <= 0) {
        free(msg);
        return NULL; 
    }
    return msg;
}

void _send_message_w(int sockfd, const char *msg, int len, struct sockaddr *addr, socklen_t addrlen) {
    int nsent = 0;

    while (nsent != len) {
        int sent = sendto(sockfd, msg + nsent, len - nsent, MSG_CONFIRM, addr, addrlen);
        if (sent == -1) {
            perror("send");
            exit(EXIT_FAILURE);
        }
        nsent += sent;
    }
}

void send_message_w(int sockfd, const char *msg, int len, struct sockaddr *addr, socklen_t addrlen) {
    char msg_len_tosend_c[SIZEBUFFER];
    snprintf(msg_len_tosend_c, SIZEBUFFER, "%d", len);
    _send_message_w(sockfd, msg_len_tosend_c, SIZEBUFFER, addr, addrlen);
    _send_message_w(sockfd, msg, len, addr, addrlen);
}

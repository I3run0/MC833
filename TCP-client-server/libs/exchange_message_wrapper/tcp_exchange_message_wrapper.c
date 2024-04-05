#include "tcp_exchange_message_wrapper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int _recv_message_w(int sockfd, char *buffer, int len) {
    ssize_t r = 0;
    ssize_t nrecv = 0;

    while (nrecv < len) {
        r = recv(sockfd, buffer + nrecv, len - nrecv, 0);
        if (r <= 0) {
            return r;
        }
        nrecv += r;
    }

    buffer[nrecv] = '\0'; // Ensure null termination
    return nrecv;
}

char *recv_message_w(int sockfd) {
    int msg_len_torecv_i;
    char msg_len_torecv_c[SIZEBUFFER];

    if (_recv_message_w(sockfd, msg_len_torecv_c, SIZEBUFFER) <= 0) {
        return NULL;
    }
    msg_len_torecv_i = atoi(msg_len_torecv_c);

    char *msg = (char *)malloc((msg_len_torecv_i + 1) * sizeof(char));
    if (_recv_message_w(sockfd, msg, msg_len_torecv_i) <= 0) {
        free(msg);
        return NULL;
    }
    return msg;
}

void _send_message_w(int sockfd, const char *msg, int len) {
    int nsent = 0;

    while (nsent != len) {
        int sent = send(sockfd, msg + nsent, len - nsent, 0);
        if (sent == -1) {
            perror("send");
            exit(EXIT_FAILURE);
        }
        nsent += sent;
    }
}

void send_message_w(int sockfd, const char *msg, int len) {
    char msg_len_tosend_c[SIZEBUFFER];
    snprintf(msg_len_tosend_c, SIZEBUFFER, "%d", len);
    _send_message_w(sockfd, msg_len_tosend_c, SIZEBUFFER);
    _send_message_w(sockfd, msg, len);
}

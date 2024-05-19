#include "udp_message_exchange_wrapper.h"

// Receive a message with a timeout
int _recv_message_w(int sockfd, char *buffer, int len, struct sockaddr *addr, socklen_t *addrlen) {
    ssize_t r = 0;
    ssize_t nrecv = 0;

    // Continue receiving until the entire message is received
    while (nrecv < len) {
        r = recvfrom(sockfd, buffer + nrecv, len - nrecv, MSG_WAITALL, addr, addrlen);
        if (r <= 0) {
            return r;  // Return error if recvfrom fails
        }
        nrecv += r;
    }

    buffer[nrecv] = '\0';  // Ensure null termination
    return nrecv;
}

// Wrapper function to receive a message with dynamic memory allocation
char *recv_message_w(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int msg_len_torecv_i;
    char msg_len_torecv_c[BUFFERSIZE];

    // Receive the length of the message
    if (_recv_message_w(sockfd, msg_len_torecv_c, BUFFERSIZE, addr, addrlen) <= 0) {
        return NULL;  // Return NULL if receiving length fails
    }
    msg_len_torecv_i = atoi(msg_len_torecv_c);  // Convert length to integer

    // Allocate memory for the message
    char *msg = (char *)malloc((msg_len_torecv_i + 1) * sizeof(char));
    if (_recv_message_w(sockfd, msg, msg_len_torecv_i, addr, addrlen) <= 0) {
        free(msg);  // Free memory if receiving message fails
        return NULL; 
    }
    return msg;  // Return the received message
}

// Send a message in chunks until the entire message is sent
void _send_message_w(int sockfd, const char *msg, int len, struct sockaddr *addr, socklen_t addrlen) {
    int nsent = 0;

    while (nsent != len) {
        int sent = sendto(sockfd, msg + nsent, len - nsent, MSG_CONFIRM, addr, addrlen);
        if (sent == -1) {
            perror("send");  // Print error if sendto fails
            exit(EXIT_FAILURE);  // Exit if sending fails
        }
        nsent += sent;
    }
}

// Wrapper function to send a message with its length
void send_message_w(int sockfd, const char *msg, int len, struct sockaddr *addr, socklen_t addrlen) {
    char msg_len_tosend_c[BUFFERSIZE];
    snprintf(msg_len_tosend_c, BUFFERSIZE, "%d", len);  // Convert message length to string
    _send_message_w(sockfd, msg_len_tosend_c, BUFFERSIZE, addr, addrlen);  // Send the length of the message
    _send_message_w(sockfd, msg, len, addr, addrlen);  // Send the actual message
}

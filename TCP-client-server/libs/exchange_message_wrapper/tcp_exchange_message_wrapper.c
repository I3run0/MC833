#include "tcp_exchange_message_wrapper.h"

int _recv_message_w(int sockfd, char * buffer, int len) {
	ssize_t r = 0;
	ssize_t nrecv = 0;

	while (nrecv < len) {
		r = recv(sockfd, buffer + nrecv, len - nrecv, 0);
		if (r <= 0) return r; 
		nrecv += r;
	}

	buffer[nrecv] += '\0';
	return nrecv;
}

char * recv_message_w(int sockfd) {
	int msg_len_torecv_i;
	char msg_len_torecv_c[SIZEBUFFER];

	if (_recv_message_w(sockfd, msg_len_torecv_c, SIZEBUFFER) <= 0) return NULL;
	msg_len_torecv_i = atoi(msg_len_torecv_c);

	char *msg = (char *) malloc((msg_len_torecv_i + 1) * sizeof(char));
	if (_recv_message_w(sockfd, msg, msg_len_torecv_i) <= 0) return NULL;
	return msg;
}

void _send_message_w(int sockfd, char *msg, int len) {
	int nsent = 0;

	char *msgtosend = (char *)malloc((len + 1)  * sizeof(char));
	strcpy(msgtosend, msg);

	while (nsent != len) {
        nsent += send(sockfd, msgtosend + nsent, len - nsent, 0); 
    }	
	
	free(msgtosend);
}

void send_message_w(int sockfd, char *msg, int len) {
	char msg_len_tosend_c[SIZEBUFFER];
	
	sprintf(msg_len_tosend_c, "%d", len);
	_send_message_w(sockfd, msg_len_tosend_c, SIZEBUFFER);
	_send_message_w(sockfd, msg, len);
}
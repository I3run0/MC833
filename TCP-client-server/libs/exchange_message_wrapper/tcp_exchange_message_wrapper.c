#include "tcp_exchange_message_wrapper.h"

struct message * create_message_w() {
    struct message * msg = (struct message *) malloc(sizeof(struct message));
    if (msg == NULL) return NULL;
	msg->message = (char *) malloc(1000 * sizeof(char));
	msg->len = 0;
    return msg;
}

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

int recv_message_w(int sockfd, struct message *msg) {
	int msg_len_torecv_i;
	char msg_len_torecv_c[SIZEBUFFER];

	if (_recv_message_w(sockfd, msg_len_torecv_c, SIZEBUFFER) <= 0) return 0;
	msg_len_torecv_i = atoi(msg_len_torecv_c);

	char *new_message = (char *) malloc((msg_len_torecv_i + 1) * sizeof(char));
	if (_recv_message_w(sockfd, new_message, msg_len_torecv_i) <= 0) return 0;

	char *tofree = msg->message;
	msg->message = new_message;
	msg->len = msg_len_torecv_i;
	free(tofree);
}

void _send_message_w(int sockfd, char *msg, int len) {
	int nsent = 0;

	char *msgtosend = (char *)malloc(len * sizeof(char));
	strcpy(msgtosend, msg);

	while (nsent != len) {
        nsent += send(sockfd, msgtosend + nsent, len - nsent, 0); 
    }	

	free(msgtosend);
}

void send_message_w(int sockfd, struct message *msg) {
	char msg_len_tosend_c[SIZEBUFFER];
	
	sprintf(msg_len_tosend_c, "%d", msg->len);
	_send_message_w(sockfd, msg_len_tosend_c, SIZEBUFFER);
	_send_message_w(sockfd, msg->message, msg->len);
}

int free_message_w(struct message *msg) {
	free(msg->message);
	free(msg);
}
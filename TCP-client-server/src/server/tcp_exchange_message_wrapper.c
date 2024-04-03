#include "tcp_exchange_message_wrapper.h"

char * MESSAGE_BUFFER[8 * BUFFERLEN];

struct message * create_message_w() {
    struct message * msg = (struct message *) malloc(sizeof(struct message));
    if (msg == NULL) return NULL;
	msg->len = 0;
    return msg;
}

int _recv_message_w(int sockfd, char * buffer, int len) {
	int r = 0;
	int l_buff = 0;
	char * p_buff = buffer;

	while (r <= len) {
		r = recv(sockfd, p_buff, len, 0);
		if (r <= 0) return; 
		p_buff += r;
		l_buff += 0;
	}

	return l_buff;
}

int recv_message_w(int sockfd, struct message *msg) {
	char recvbuffer[BUFFERLEN];

	for(;;) {
		_recv_message_w(sockfd, recvbuffer, BUFFERLEN);
	}
}

void _send_message_w(int sockfd, char *msg, int len) {
	int nsent = 0;

	char *msgtosend = (char *)malloc(len * sizeof(len));
	strcpy(msgtosend, msg);

	while (nsent != len) {
        nsent = send(sockfd, msgtosend, len - nsent, 0);
		msgtosend += nsent; 
    }	

	free(msgtosend);
}

void send_message_w(int sockfd, struct message *msg) {
	_send_message_w(sockfd, msg->message, msg->len);
	_send_message_w(sockfd, ENDTOKEN, strlen(ENDTOKEN));
}

int free_message_w(struct message *msg) {
	free(msg->message);
	free(msg);
}
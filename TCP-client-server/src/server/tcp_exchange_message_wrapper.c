#include "tcp_exchange_message_wrapper.h"

int init_message_w(struct message * msg) {
    msg = (struct message *)malloc(sizeof(struct message));
    if (msg == NULL) return 0;
    msg->message = (char *)malloc(BUFFERLEN  * sizeof(char));
    msg->len = 0;
    return 1;
}

int recv_message_w(int sockfd, struct message *msg) {
	int rn = 0, sn = 0, message_buff_size =0;
	char recv_buffer[BUFFERLEN];
	char message_buffer[BUFFERLEN];

	for(;;) {
		rn = recv(sockfd, recv_buffer, BUFFERLEN, 0);
		
		if (rn < 0) {
			return rn;
		} else if (rn == 0) {
			return rn;
		}

		printf("recv: %s", recv_buffer);
		memcpy(message_buffer + message_buff_size, recv_buffer, strlen(recv_buffer));
		message_buff_size += strlen(recv_buffer);
		message_buffer[message_buff_size] = '\0';
		printf("message: %ld %s", strlen(message_buffer), message_buffer);
		
		char *message = NULL;
        int message_size;
		char *end_message = strstr(message_buffer, "\r\r");
		if (end_message != NULL) {
			message_size = end_message - message_buffer;
			message = (char *) malloc(message_size + 1);
			memcpy(message, message_buffer, message_size);
			message[message_size + 1] = '\0';
			memmove(message_buffer, end_message, message_buff_size - message_size + 2);
			message_buff_size -= message_size + 2;
			printf("size: %d\n", message_size);

		}
		printf("pointer: %p\n", end_message);
		if (message != NULL) {
            strcpy(msg->message, message);
            msg->len = message_size;
            return 1;
		}
    }

    return 1;
}

void _send_message_w(int sockfd, char *msg, int len) {
	int nsent = 0;

	char *msgtosend = (char *)malloc(len * sizeof(len));
	strcpy(msgtosend, msg);

	while (nsent != len) {
        nsent += send(sockfd, msgtosend, len - nsent, 0);
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
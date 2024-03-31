#include "request_hendler.h"

void request_hendler(int socket) {
	int rn = 0, sn = 0, request_buff_size =0;
	char recv_buffer[BUFFERLEN];
	char request_buffer[BUFFERLEN];

	for(;;) {
		rn = recv(socket, recv_buffer, BUFFERLEN, 0);
		
		if (rn < 0) {
			perror("Error reading from socket\n");
			return;
		} else if (rn == 0) {
			printf("Client closed connection\n");
			return;
		}

		printf("recv: %s", recv_buffer);
		memcpy(request_buffer + request_buff_size, recv_buffer, strlen(recv_buffer));
		request_buff_size += strlen(recv_buffer);
		request_buffer[request_buff_size] = '\0';
		printf("request: %ld %s", strlen(request_buffer), request_buffer);
		
		char *request = NULL;
		char *end_request = strstr(request_buffer, "\r\r");
		if (end_request != NULL) {
			int request_size = end_request - request_buffer;
			request = (char *) malloc(request_size + 1);
			memcpy(request, request_buffer, request_size);
			request[request_size + 1] = '\0';
			memmove(request_buffer, end_request, request_buff_size - request_size + 2);
			request_buff_size -= request_size + 2;
			printf("size: %d\n", request_size);

		}
		printf("pointer: %p\n", end_request);
		if (request != NULL) {
			printf("send_request: %s\n", request);

			char *response = process_request(request);
			send(socket, response, BUFFERLEN, 0);
			request_buff_size = 0;
		}
	}

}

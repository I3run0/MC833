#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define ENDTOKEN "\n\r\n\r"

int main(int argc, char *argv[]) {
    int socketfd, rv;
    struct addrinfo *serveaddr, hints, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &serveaddr)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = serveaddr; p != NULL; p = p->ai_next) {
        if ((socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
        perror("client: socket");
        continue;
    }

    if (connect(socketfd, p->ai_addr, p->ai_addrlen) == -1) {
        close(socketfd);
        perror("client: connect");
        continue;
    }

    break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
    }

    printf("Client connect to %s\n", argv[1]);

    int nsend = 0, msg_size = 0;
    char msg[BUF_SIZE];
    char recv_msg[BUF_SIZE];
    for(;;) { 
	nsend = 0;
	printf(">>> ");
        fgets(msg, BUF_SIZE, stdin);
	printf("%ld %s", strlen(msg), msg);
        while(nsend != BUF_SIZE) {
            nsend = send(socketfd, msg, BUF_SIZE, 0);
            //printf("client: sent %d/%d\n", nsend, BUF_SIZE);
	}
	send(socketfd, "\r\r", BUF_SIZE, 0);
        recv(socketfd, recv_msg, BUF_SIZE, 0);
        printf("%s", recv_msg);
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../../libs/exchange_message_wrapper/tcp_exchange_message_wrapper.h"

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
    char msg[2000];
    char recv_msg[200];
    for(;;) { 
        nsend = 0;
        printf(">>> ");
        fgets(msg, 2000, stdin);
        struct message *msg_struc = create_message_w();
        strcpy(msg_struc->message, msg);
        msg_struc->len = strlen(msg);
        //printf("%ld %s", msg_struc->len, msg_struc->message);
        send_message_w(socketfd, msg_struc);
        recv_message_w(socketfd, msg_struc);
        printf("%s", msg_struc->message);

    }
    return 0;
}

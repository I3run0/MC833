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
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &serveaddr)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = serveaddr; p != NULL; p = p->ai_next) {
        if ((socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        exit(1);
    }

    printf("Client connected to %s\n", argv[1]);

    char request[2000];
    char *response;

    for (;;) {
        printf(">>> ");
        fgets(request, 2000, stdin);

        send_message_w(socketfd, request, strlen(request), p->ai_addr, p->ai_addrlen);
        response = recv_message_w(socketfd, p->ai_addr, &(p->ai_addrlen));

        if (strcmp(response, "INSERT") == 0) {
            /* TO DO */
        } else if (strcmp(response, "DOWNLOAD") == 0) {
            /* TO DO */
        }

        printf("%s", response);
        free(response);
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../libs/udp_message_exchange_wrapper.h"
#include "../../libs/udp_file_exchange_wrapper.h"

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
    char *response, *opt, *rspns_dup;
    for (;;) {
        printf(">>> ");
        fgets(request, 2000, stdin);
        send_message_w(socketfd, request, strlen(request), p->ai_addr, p->ai_addrlen);
        response = recv_message_w(socketfd, p->ai_addr, &(p->ai_addrlen));
        if (!response) {
            printf("\nSERVER not responde\n\n");
            exit(1);
        }
        rspns_dup = strdup(response);
        opt = strtok(rspns_dup, " ");
        if(strcmp(opt, "INSERT") == 0) {
            char *path;
            path = strtok(NULL, "\0");
            send_file_w(socketfd, path, p->ai_addr, p->ai_addrlen);
            printf("\nINSERT %s\n\n", path);
        } else if (strcmp(opt, "DOWNLOAD") == 0) {
            char *server_path, local_path[512];
            server_path = strtok(NULL, "\0");
            sscanf(server_path, "server_data/storage/%s", server_path);
            strcpy(local_path, "client_data/storage_download/");
            strcat(local_path, server_path);
            recv_file_w(socketfd, local_path, p->ai_addr, p->ai_addrlen);
            printf("\nDOWNLOAD in %s\n\n", local_path);
        } else {
            printf("%s", response);
        }
        free(response);
        free(rspns_dup);
    }
    return 0;
}

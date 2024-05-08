#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sqlite3.h>

#include "../../libs/exchange_message_wrapper/tcp_exchange_message_wrapper.h"
#include "process_request.h"

#define PORT "3490"  // the port users will be connecting to
#define BACKLOG 10   // how many pending connections queue will hold
#define RESPONSE_BUFFER_SIZE 1024

void sigchld_handler(int s) {
    (void)s; // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// get sin_port, IPv4 or IPv6:
uint16_t get_in_port(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return (((struct sockaddr_in*)sa)->sin_port);
    }
    return (((struct sockaddr_in6*)sa)->sin6_port);
}

sqlite3 *get_db_instance(char *db_path) {
    sqlite3 *db;
    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    if (create_music_table(db)) {
        return NULL;
    }

    return db;
}

int main(int argc, char *argv[]) {
    sqlite3 *server_database;
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <database_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    server_database = get_db_instance(argv[1]);
    if (server_database == NULL) {
        fprintf(stderr, "Failed to initialize the database\n");
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(EXIT_FAILURE);
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("server: waiting for connections...\n");

    while (1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        int port = ntohs(get_in_port((struct sockaddr *)&their_addr));
        printf("server: got connection from %s:%d\n", s, port);

        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            char *msg;
            char *response;
            for (;;) {
                msg = recv_message_w(new_fd);
                if (msg == NULL) break;
                response = process_request(server_database, msg);
                send_message_w(new_fd, response, strlen(response));
                free(msg);
                free(response);
            }
            
            printf("server: finished connection with %s:%d\n", s, port);
            close(new_fd);
            exit(EXIT_SUCCESS);
        }
        close(new_fd);  // parent doesn't need this
    }

    return 0;
}

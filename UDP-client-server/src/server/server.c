#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "database.h"
#include "../../libs/exchange_message_wrapper/tcp_exchange_message_wrapper.h"

#define RESPONSE_BUFFER_SIZE 100000
#define MAXPATHLEN 1000

void insert_request(int sockfd, struct sockaddr *addr, char *request, sqlite3 *db) {
    char response[RESPONSE_BUFFER_SIZE];

    char *data = strtok(NULL, "\n");
    if (!data) {
        strpy(response, "\nERROR: INSERT: No data provided for insertion\n\n");
        send_message_w(sockfd, response, strlen(response), addr, sizeof(addr));
        return;
    }

    char toinsert[strlen(data) +  MAXPATHLEN];
    strcpy(toinsert, data)
    int result = insert_music(db, data);
    if (result != SQLITE_OK) {
        strcpy(response, "\nERROR: INSERT: Failed to insert data into the database\n\n");
        send_message_w(sockfd, response, strlen(response), addr, sizeof(addr));
        return;
    }

    stcpy(response, "INSERT"); // Init the file transferetion
    send_message_w(sockfd, response, strlen(response), addr, sizeof(addr));
    char *path = extract_last_value(request);

}

void delete_request(int sockfd, struct sockaddr *addr, char *request, sqlite3 *db) {
    char *id = strtok(NULL, "\n");
    char response[RESPONSE_BUFFER_SIZE]; 
    if (!id) {
        strcpy(response, "\nERROR: DELETE: No ID provided for deletion\n\n");
        send_message_w(sockfd, response, strlen(response), addr, sizeof(addr));
        return;
    }
    int result = delete_music(db, id);
    if (result != SQLITE_OK) {
        strcpy(response, "\nERROR: DELETE: Failed to delete data from the database\n\n");
        send_message_w(sockfd, response, strlen(response), addr, sizeof(addr));
        return;
    }
    strcpy(response, "\nSUCCESS: DELETE: Data deleted successfully\n\n");
    send_message_w(sockfd, response, strlen(response), addr, sizeof(addr));
}

void select_request(int sockfd, struct sockaddr *addr, char *request, sqlite3 *db) {
    char *to_filter = NULL;
	char *to_select = strtok(NULL, "WHERE");
    char *error = "\nERROR: SELECT: Failed to retrieve data from the database\n\n";

	if (!to_select) {
		to_select = strtok(NULL, "\n");
	} else {
		to_filter = strtok(NULL, " ");
		to_filter = strtok(NULL, "\n");
	}
    char *response = (char *)malloc(RESPONSE_BUFFER_SIZE * sizeof(char));
    if (!response) {
        send_message_w(sockfd, error, strlen(error), addr, sizeof(addr));
        return;
    }

    int result = select_music(db, to_select, to_filter, response);
    if (result < 0) {
        free(response);
        send_message_w(sockfd, error, strlen(error), addr, sizeof(addr));
        return;
    } else if (result == 0) {
        char *nodata = "\nSELECT: There are no data in music database\n\n";
        return;
    }
    send_message_w(sockfd, response, strlen(response), addr, sizeof(addr));;
}


void help_request(int sockfd, struct sockaddr *addr) {
    char* response = "Available operations:\n"
                         "INSERT: Insert data into the database\n"
                         "    Syntax: INSERT '<id>', '<titulo>', '<interprete>', '<idioma>', '<tipo_de_musica>', '<refrao>', '<ano_de_lancamento>', \n"
                         "    Examples:\n"
                         "        INSERT 'a', 'b', 'c', 'd', 'f', 'g', 'h'\n"
                         "DELETE: Delete data from the database\n"
                         "    Syntax: DELETE <id>\n"
                         "SELECT: Select data from the database\n"
                         "    Syntax: SELECT columns WHERE filter\n"
                         "            OR\n"
                         "            SELECT columns\n"
                         "    Examples:\n"
                         "        SELECT * WHERE id='<value>'\n"
                         "        SELECT title, artist WHERE genre='<value>'\n"
                         "\n";
    send_message_w(sockfd, response, strlen(response), addr, sizeof(addr));
}

void wrong_request(int sockfd, struct sockaddr *addr) {
    char *response = "\nERROR: TO_PROCESS_REQUEST: Invalid operation specified in the request\n\n";
    send_message_w(sockfd, response, strlen(response), addr, sizeof(addr));
}

void handle_request(sqlite3 *db, char *request, int sockfd, struct sockaddr *addr) {
    char *local_request = strdup(request);
    char *op = strtok(local_request, " ");

    /* Select the request */
    if (strcmp(op, "HELP") == 0) {
        help_request(sockfd, addr);
    } else if (strcmp(op, "INSERT") == 0) {
        insert_request(sockfd, addr, local_request, db);
    } else if (strcmp(op, "SELECT") == 0) {
        select_request(sockfd, addr, local_request, db);
    } else if (strcmp(op, "DELETE") == 0) {
        delete_request(sockfd, addr, local_request, db);
    } else if (strcmp(op, "DOWNLOAD") == 0) {
        dowload_request(sockfd, addr, local_request, db);
    } else {
        wrong_request(sockfd, addr);
    }
}

#define MYPORT "4950" // the port users will be connecting to

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr client_addr;
    socklen_t client_addrlen;
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    printf("listener: waiting to recvfrom...\n");

    while(1) {
        char* msg = recv_message_w(sockfd, &client_addr, &client_addrlen);
        send_message_w(sockfd, msg, strlen(msg), &client_addr, client_addrlen);
    }
    close(sockfd);

    return 0;
}


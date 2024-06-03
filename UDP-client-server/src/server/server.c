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
#include "../../libs/udp_message_exchange_wrapper.h"
#include "../../libs/udp_file_exchange_wrapper.h"

#define RESPONSE_BUFFER_SIZE 100000
#define MAXFIELDSLEN 256

void insert_request(int sockfd, struct sockaddr *addr, socklen_t addrlen, char *request, sqlite3 *db) {
    int res;
    char id[MAXFIELDSLEN], title[MAXFIELDSLEN], singer[MAXFIELDSLEN],
        language[MAXFIELDSLEN], type[MAXFIELDSLEN], lyrics[MAXFIELDSLEN],
        release_date[MAXFIELDSLEN], storage_path[MAXFIELDSLEN * 2];
    
    char* local_request = strdup(request);
    char response[RESPONSE_BUFFER_SIZE];
    res = sscanf(request, "INSERT '%99[^']' '%99[^']' '%99[^']' '%99[^']' '%99[^']' '%99[^']' '%99[^']' '%199[^']'\n",
                 id, title, singer, language, type, lyrics, release_date, storage_path);
    if (res < 8) { 
        strcpy(response, "\nERROR: INSERT: No data provided or data absent for insertion\n\n");
        send_message_w(sockfd, response, strlen(response), addr, addrlen);
        return;
    }

    char local_storage_path[MAXFIELDSLEN * 2];
    sprintf(local_storage_path, "server_data/storage/%s.mp3", id);
    char* data_to_insert = (char *)malloc(strlen(local_request) + strlen(local_storage_path) + 1);
    sprintf(data_to_insert, "'%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s'",  
                id, title, singer, language, type, lyrics, release_date, local_storage_path);

    int result = insert_music(db, data_to_insert);
    if (result != SQLITE_OK) {
        strcpy(response, "\nERROR: INSERT: Failed to insert data into the database\n\n");
        send_message_w(sockfd, response, strlen(response), addr, addrlen);
        return;
    }

    sprintf(response, "INSERT %s", storage_path); // Init the file transferetion
    send_message_w(sockfd, response, strlen(response), addr, addrlen);
    recv_file_w(sockfd, local_storage_path, addr, addrlen);
    free(local_request);
}

void download_request(int sockfd, struct sockaddr *addr, socklen_t addrlen, char* request, sqlite3 *db) {
    char *id = strtok(NULL, "\n");
    char response[RESPONSE_BUFFER_SIZE];
    char path[256];
    if (!id) {
        strcpy(response, "\nERROR: DOWNLOAD: No ID provided for download\n\n");
        send_message_w(sockfd, response, strlen(response), addr, addrlen);
        return;
    }

    if (select_path_by_id(db, id, path) < 0) {
        strcpy(response, "\nERROR: DOWNLOAD: ID not found\n\n");
        send_message_w(sockfd, response, strlen(response), addr, addrlen);
        return;
    }

    if (access(path, F_OK) != 0) {
        strcpy(response, "\nERROR: DOWNLOAD: File associeted to id not foud\n\n");
        send_message_w(sockfd, response, strlen(response), addr, addrlen);
        return;
    }

    /* Init the file transfertion */
    sprintf(response, "DOWNLOAD %s", path);
    send_message_w(sockfd, response, strlen(response), addr, addrlen);
    send_file_w(sockfd, path, addr, addrlen);
}

void delete_request(int sockfd, struct sockaddr *addr, socklen_t addrlen, char* request, sqlite3 *db) {
    char *id = strtok(NULL, "\n");
    char response[RESPONSE_BUFFER_SIZE];
    char path[MAXFIELDSLEN];

    if (!id) {
        strcpy(response, "\nERROR: DELETE: No ID provided for deletion\n\n");
        send_message_w(sockfd, response, strlen(response), addr, addrlen);
        return;
    }

    if (select_path_by_id(db, id, path) < 0) {
        strcpy(response, "\nERROR: DELETE: ID not found\n\n");
        send_message_w(sockfd, response, strlen(response), addr, addrlen);
        return;
    }

    if (access(path, F_OK) == 0) {
        if (remove(path) != 0) {
            strcpy(response, "\nERROR: DELETE: Is not possible delete the song file\n\n");
            send_message_w(sockfd, response, strlen(response), addr, addrlen);
            return;
        }

    }

    int result = delete_music(db, id);
    if (result != SQLITE_OK) {
        strcpy(response, "\nERROR: DELETE: Failed to delete data from the database\n\n");
        send_message_w(sockfd, response, strlen(response), addr, addrlen);
        return;
    }
    strcpy(response, "\nSUCCESS: DELETE: Data deleted successfully\n\n");
    send_message_w(sockfd, response, strlen(response), addr, addrlen);
}

void select_request(int sockfd, struct sockaddr *addr, socklen_t addrlen, char *request, sqlite3 *db) {
    char *to_filter = NULL;
	char *to_select = strtok(NULL, "WHERE");
    char *error = "\nERROR: SELECT: Failed to retrieve data from the database\n\n";

	if (!to_select) {
		to_select = strtok(NULL, "\n");
	} else {
		to_filter = strtok(NULL, " ");
		to_filter = strtok(NULL, "\n");
	}

    char response[RESPONSE_BUFFER_SIZE];
    int result = select_music(db, to_select, to_filter, response);
    if (result < 0) {
        strcpy(response, error);
    } else if (result == 0) {
        strcpy(response, "\nSELECT: There are no data in music database\n\n");
    }

    send_message_w(sockfd, response, strlen(response), addr, addrlen);
}


void help_request(int sockfd, struct sockaddr *addr, socklen_t addrlen) {
    char* response = "Available operations:\n"
                         "INSERT: Insert data into the database\n"
                         "    Syntax: INSERT '<id>', '<titulo>', '<interprete>', '<idioma>', '<tipo_de_musica>', '<refrao>', '<ano_de_lancamento>', '<caminho_do_arquivo>' \n"
                         "    Examples:\n"
                         "        INSERT 'a', 'b', 'c', 'd', 'f', 'g', 'h', 'path/to/the/music.mp3\n"
                         "DELETE: Delete data from the database\n"
                         "    Syntax: DELETE '<id>'\n"
                         "SELECT: Select data from the database\n"
                         "    Syntax: SELECT columns WHERE filter\n"
                         "            OR\n"
                         "            SELECT columns\n"
                         "    Available columns: id, titulo, interprete, idioma, tipo_de_musica, refrao, ano_de_lancamento, caminho_do_arquivo\n"
                         "    Examples:\n"
                         "        SELECT * WHERE id='<value>'\n"
                         "        SELECT titulo, interprete, WHERE tipo_de_musica='rock'\n"
                         "DOWNLOAD: Download data from the database\n"
                         "    Syntax: DOWNLOAD '<id>'\n"
                         "\n";
    send_message_w(sockfd, response, strlen(response), addr, addrlen);
}

void wrong_request(int sockfd, struct sockaddr *addr, socklen_t addrlen) {
    char *response = "\nERROR: TO_PROCESS_REQUEST: Invalid operation specified in the request\n\n";
    send_message_w(sockfd, response, strlen(response), addr, addrlen);
}

void hendle_request(int sockfd, struct sockaddr *addr, socklen_t addrlen, char *request, sqlite3 *db) {
    char *local_request = strdup(request);
    char *op = strtok(local_request, " \n");
    if (!op) {
        wrong_request(sockfd, addr, addrlen);
        return;
    }
    /* Select the request */
    if (strcmp(op, "HELP") == 0) {
        help_request(sockfd, addr, addrlen);
    } else if (strcmp(op, "INSERT") == 0) {
        insert_request(sockfd, addr, addrlen, request, db);
    } else if (strcmp(op, "SELECT") == 0) {
        select_request(sockfd, addr, addrlen, local_request, db);
    } else if (strcmp(op, "DELETE") == 0) {
        delete_request(sockfd, addr, addrlen, local_request, db);
    } else if (strcmp(op, "DOWNLOAD") == 0) {
        download_request(sockfd, addr, addrlen, local_request, db);
    } else {
        wrong_request(sockfd, addr, addrlen);
    }
    free(request);
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

#define MYPORT "4950" // the port users will be connecting to

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
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

int main(int argc, char *argv[]) {
    sqlite3 *server_database;
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    struct sockaddr_storage client_addr;
    socklen_t client_addrlen;
    char addrs[INET6_ADDRSTRLEN];

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
        char* msg = recv_message_w(sockfd, (struct sockaddr *)&client_addr, &client_addrlen);
        inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *)&client_addr), addrs, sizeof addrs);
        int port = ntohs(get_in_port((struct sockaddr *)&client_addr));
        printf("server: got connection from %s:%d\n", addrs, port);
        hendle_request(sockfd, (struct sockaddr *)&client_addr, client_addrlen, msg, server_database);
    }
    close(sockfd);

    return 0;
}


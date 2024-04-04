#include "process_request.h"
#define RESPONSE_BUFFER_SIZE 4000

char* insert_d(char *request, sqlite3 *db) {
    char *data = strtok(NULL, "\n");
    if (!data) {
        return strdup("ERROR: No data provided for insertion\n");
    }
    int result = insert_music(db, data);
    if (result != SQLITE_OK) {
        return strdup("ERROR: Failed to insert data into the database\n");
    }
    return strdup("SUCCESS: Data inserted successfully\n");
}

char* delete_d(char *request, sqlite3 *db) {
    char *id = strtok(NULL, "\n");
    if (!id) {
        return strdup("ERROR: No ID provided for deletion\n");
    }
    int result = delete_music(db, id);
    if (result != SQLITE_OK) {
        return strdup("ERROR: Failed to delete data from the database\n");
    }
    return strdup("SUCCESS: Data deleted successfully\n");
}

char* select_d(char *request, sqlite3 *db) {
    char *to_filter = NULL;
    char *to_select = strtok(NULL, "WHERE");

    if (!to_select) {
        to_select = strtok(NULL, "\n");
    } else {
        to_filter = strtok(NULL, " ");
        to_filter = strtok(NULL, "\n");
    }

    char *response = malloc(RESPONSE_BUFFER_SIZE * sizeof(char));
    if (!response) {
        return strdup("ERROR: Memory allocation failed for response buffer\n");
    }

    int result = select_music(db, to_select, to_filter, response);
    if (result != SQLITE_OK) {
        free(response);
        return strdup("ERROR: Failed to retrieve data from the database\n");
    }

    return response;
}

char* process_request(sqlite3 *db, char *request) {
	char *response;
    char *local_request = strdup(request);
    if (!local_request) {
        response = strdup("ERROR: Memory allocation failed for local request buffer\n");
    }

    char *op = strtok(local_request, " ");
    if (!op) {
        response = strdup("ERROR: Invalid operation specified in the request\n");
    }

    if (strcmp(op, "INSERT") == 0) {
        response = insert_d(local_request, db);
    } else if (strcmp(op, "SELECT") == 0) {
        response = select_d(local_request, db);
    } else if (strcmp(op, "DELETE") == 0) {
		response = delete_d(local_request, db);
    } else {
		response = strdup("ERROR: Invalid operation specified in the request\n");
	}

    free(local_request);
    return response;
}

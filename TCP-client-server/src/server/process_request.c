#include "process_request.h"
#define RESPONSE_BUFFER_SIZE 1024

char* insert_d(char *request, sqlite3 *db) {
    char *data = strtok(NULL, "\n");
    if (!data) {
        return strdup("ERROR: INSERT: No data provided for insertion\n");
    }
    int result = insert_music(db, data);
    if (result != SQLITE_OK) {
        return strdup("ERROR: INSERT: Failed to insert data into the database\n");
    }
    return strdup("SUCCESS: INSERT: Data inserted successfully\n");
}

char* delete_d(char *request, sqlite3 *db) {
    char *id = strtok(NULL, "\n");
    if (!id) {
        return strdup("ERROR: DELETE: No ID provided for deletion\n");
    }
    int result = delete_music(db, id);
    if (result != SQLITE_OK) {
        return strdup("ERROR: DELETE: Failed to delete data from the database\n");
    }
    return strdup("SUCCESS: DELETE: Data deleted successfully\n");
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
    char *error =  strdup("ERROR: SELECT: Failed to retrieve data from the database\n");
    char *response = malloc(RESPONSE_BUFFER_SIZE * sizeof(char));
    if (!response) {
        return error;
    }
    int result = select_music(db, to_select, to_filter, response);
    if (result != SQLITE_OK) {
        free(response);
        return error;
    }
    if (response == NULL)
        return error;
    return response;
}

char* help_d() {
    char* help_message = "HELP_D:\n"
                         "Available operations:\n"
                         "INSERT: Insert data into the database\n"
                         "    Syntax: INSERT data\n"
                         "DELETE: Delete data from the database\n"
                         "    Syntax: DELETE id\n"
                         "SELECT: Select data from the database\n"
                         "    Syntax: SELECT columns WHERE filter\n"
                         "            OR\n"
                         "            SELECT columns\n"
                         "    Examples:\n"
                         "        SELECT * WHERE id='5'\n"
                         "        SELECT title, artist WHERE genre='Rock'\n";
    return strdup(help_message);
}

char* process_request(sqlite3 *db, char *request) {
	char *response;
    char *local_request = strdup(request);
    if (!local_request) {
        response = strdup("ERROR: TO_PROCESS_REQUEST Memory allocation failed for local request buffer\n");
    }

    char *op = strtok(local_request, " ");
    if (!op) {
        response = strdup("ERROR: TO_PROCESS_REQUEST: Invalid operation specified in the request\n");
    }

    if (strcmp(op, "INSERT") == 0) {
        response = insert_d(local_request, db);
    } else if (strcmp(op, "SELECT") == 0) {
        response = select_d(local_request, db);
    } else if (strcmp(op, "DELETE") == 0) {
		response = delete_d(local_request, db);
    } else {
		response = strdup("ERROR: TO_PROCESS_REQUEST: Invalid operation specified in the request\n");
	}

    free(local_request);
    return response;
}

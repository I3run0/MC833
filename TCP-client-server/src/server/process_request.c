#include "process_request.h"
#define RESPONSE_BUFFER_SIZE 100000

char* insert_d(char *request, sqlite3 *db) {
    char *data = strtok(NULL, "\n");
    if (!data) {
        return strdup("\nERROR: INSERT: No data provided for insertion\n\n");
    }
    int result = insert_music(db, data);
    if (result != SQLITE_OK) {
        return strdup("\nERROR: INSERT: Failed to insert data into the database\n\n");
    }
    return strdup("\nSUCCESS: INSERT: Data inserted successfully\n\n");
}

char* delete_d(char *request, sqlite3 *db) {
    char *id = strtok(NULL, "\n");
    if (!id) {
        return strdup("\nERROR: DELETE: No ID provided for deletion\n\n");
    }
    int result = delete_music(db, id);
    if (result != SQLITE_OK) {
        return strdup("\nERROR: DELETE: Failed to delete data from the database\n\n");
    }
    return strdup("\nSUCCESS: DELETE: Data deleted successfully\n\n");
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
    char *error =  strdup("\nERROR: SELECT: Failed to retrieve data from the database\n\n");
    char *response = (char *)malloc(RESPONSE_BUFFER_SIZE * sizeof(char));
    if (!response) {
        return error;
    }

    int result = select_music(db, to_select, to_filter, response);
    if (result < 0) {
        free(response);
        return error;
    } else if (result == 0) {
        return strdup("\nSELECT: There are no data in music database\n\n");
    }
    return response;
}

char* help_d() {
    char* help_message = "Available operations:\n"
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
    return strdup(help_message);
}

char* process_request(sqlite3 *db, char *request) {
	char *response;
    char *local_request = strdup(request);
    if (!local_request) {
        response = strdup("\nERROR: TO_PROCESS_REQUEST Memory allocation failed for local request buffer\n\n");
    }

    char *op = strtok(local_request, " ");
    if (!op) {
        response = strdup("\nERROR: TO_PROCESS_REQUEST: Invalid operation specified in the request\n\n");
    }

    if (strcmp(op, "HELP") == 0) {
        response = help_d();
    } else if (strcmp(op, "INSERT") == 0) {
        response = insert_d(local_request, db);
    } else if (strcmp(op, "SELECT") == 0) {
        response = select_d(local_request, db);
    } else if (strcmp(op, "DELETE") == 0) {
		response = delete_d(local_request, db);
    } else {
		response = strdup("\nERROR: TO_PROCESS_REQUEST: Invalid operation specified in the request\n\n");
	}

    free(local_request);
    return response;
}

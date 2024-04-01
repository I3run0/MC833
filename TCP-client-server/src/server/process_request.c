#include "process_request.h"

#define FIELD_SIZE 250

char* insert_d(char *request, sqlite3 *db) {
	char *data = strtok(NULL, "\n");
	printf("data: %s", data);
	int a = insert_music(db, data);
	if (a == 0) {
		return "ERROR: inserting data\n";
	}	
	return "SUCCESSFULL INSERT";
}


char* delete_d(char *request, sqlite3 *db) {
	char *id = strtok(NULL, "\n");
        if (delete_music(db, id)) 
		return "ERROR: deleting data\n";
	return "DELETE id ";
}

char* select_d(char *request, sqlite3 *db) {
	char *tofilter = NULL;
	char *toselect = strtok(NULL, "WHERE");
	
	if (toselect == NULL) {
		toselect = strtok(NULL, "\n");
	} else {
		tofilter = strtok(NULL, " ");
		tofilter = strtok(NULL, "\n");
	}

	char *response = (char *) malloc(1024 * sizeof(char));
	
	if (select_music(db, toselect, tofilter, response)) 
		return "ERROR: selecting data";


	return response;

}
char* process_request(char *request) {
	char *response = NULL;
	
	sqlite3 *db;
    	if (sqlite3_open("music.db", &db) != SQLITE_OK) {
        	fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        	return "Cannot open database";
    	}

   	if (create_music_table(db)) {
		return "ERROR ao criar DB";
	}

	char *op = strtok(request, " ");
	if (strcmp(op, "INSERT") == 0) {
		return response = insert_d(request, db);
	} else if (strcmp(op, "SELECT") == 0) {
		return response = select_d(request, db);
	} else if (strcmp(op, "DELETE") == 0) {
		return response = delete_d(request, db);
	}

	return "The commend not exist\n";
}


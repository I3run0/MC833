#include "process_request.h"

char* create(char *request) {
	char id[50], title[50];
	char *response = (char*)malloc(50 * sizeof(char));
	int rv = sscanf(request, "CREATE&& id %s &&", id);
	if (rv < 1) return "CREATE: FAILURE\n";
	sprintf(response, "id %s\n", id);
	return response;
}

char* delete(char *request) {
	return "DELETE";
}

char* get(char *request) {
	return "GET";
}

char* process_request(char *request) {
	char *response = NULL;
	
	char *op = strtok(request, "&&");
	if (strcmp(op, "CREATE") == 0) {
		return response = create(request);
	} else if (strcmp(op, "DELETE") == 0) {
		return response = delete(request);
	} else if (strcmp(op, "GET") == 0) {
		return response = get(request);
	}

	return "The commend not exist\n";
}



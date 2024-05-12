#ifndef PROCESS_REQUEST_H
#define PROCESS_REQUEST_H

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

char* process_request(sqlite3* db, char* request);

#endif /* PROCESS_REQUEST_H */

#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

char* process_request(sqlite3 *db, char *resquest);

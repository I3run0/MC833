#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

int create_music_table(sqlite3 *db);

int insert_music(sqlite3 *db, char *data);

int select_music(sqlite3 *db, char *fields, char *filter, char *result);

int delete_music(sqlite3 *db, char *id);


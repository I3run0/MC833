#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

// Function prototypes
int create_music_table(sqlite3* db);
int insert_music(sqlite3* db, const char* data);
int select_music(
    sqlite3* db,
    const char* fields,
    const char* filter,
    char* result);
int delete_music(sqlite3* db, const char* id);

#endif /* DATABASE_H */

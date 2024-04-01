#include "database.h"

// Function to create the musica table
int create_music_table(sqlite3 *db) {
    char *sql = "CREATE TABLE IF NOT EXISTS musica ("
                "id TEXT,"
                "titulo TEXT,"
                "interprete TEXT,"
                "idioma TEXT,"
                "tipo_de_musica TEXT,"
                "refrao TEXT,"
                "ano_de_lancamento TEXT"
                ");";
    if (sqlite3_exec(db, sql, 0, 0, 0) != SQLITE_OK) {
        return 0;
    }
}


// Function to insert a new musica record
int insert_music(sqlite3 *db, char *data) {
    char *sql = sqlite3_mprintf("INSERT INTO musica (id, titulo, interprete, idioma, tipo_de_musica, refrao, ano_de_lancamento) VALUES (%s);", data);
    printf("%s", sql);
    if (sqlite3_exec(db, sql, 0, 0, 0) != SQLITE_OK) {
    	return 1;
    }

    sqlite3_free(sql);

    return 0;
}

// Function to execute a GET query with filters
int select_music(sqlite3 *db, char *fields, char *filter, char *result) {
    char *sql;
    if (filter == NULL) {
    	sql = sqlite3_mprintf("SELECT %s FROM musica;", fields);
    } else {
    	sql = sqlite3_mprintf("SELECT %s FROM musica WHERE %s;", fields, filter);
    }

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        return 1;
    }

    char *resp = result;
    int len;
    while (sqlite3_step(stmt) == SQLITE_ROW) { 
	for (int i = 0; i < sqlite3_column_count(stmt); i++) {
            sprintf(resp, "%s: %s ", sqlite3_column_name(stmt, i), sqlite3_column_text(stmt, i));
            len = strlen(resp);
	    resp += (strlen(resp) * sizeof(char));
	}
	len = strlen(resp);
	resp[len] = '\n';
	resp[len + 1] = '\0';
	resp += (strlen(resp) * sizeof(char));

    }

    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return 0;
}


// Function to delete music
int delete_music(sqlite3 *db, char *id) {
    char *sql = sqlite3_mprintf("DELETE FROM musica WHERE id=%s;", id);
    if (sqlite3_exec(db, sql, 0, 0, 0) != SQLITE_OK) {
       return 1;
    }
    sqlite3_free(sql);
    return 0;
}



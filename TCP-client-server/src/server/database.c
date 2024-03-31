#include "database.h"

// Function to create the musica table
int create_music_table(sqlite3 *db) {
    char *sql = "CREATE TABLE IF NOT EXISTS musica ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
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
    char *sql = sqlite3_mprintf("INSERT INTO musica (titulo, interprete, idioma, tipo_de_musica, refrao, ano_de_lancamento) VALUES (%s);", data);
    printf("%s", sql);
    if (sqlite3_exec(db, sql, 0, 0, 0) != SQLITE_OK) {
    	return 0;
    }

    sqlite3_free(sql);

    return 1;
}

int get_rows();


int remove_rows();


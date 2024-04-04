// database.c
#include "database.h"



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
    int result = sqlite3_exec(db, sql, 0, 0, 0);
    return result;
}

int insert_music(sqlite3 *db, char *data) {
    char *sql = sqlite3_mprintf("INSERT INTO musica (id, titulo, interprete, idioma, tipo_de_musica, refrao, ano_de_lancamento) VALUES (%s);", data);
    int result = sqlite3_exec(db, sql, 0, 0, 0);
    sqlite3_free(sql);
    return result;
}

int select_music(sqlite3 *db, char *fields, char *filter, char *result) {
    char *sql;
    if (filter == NULL) {
        sql = sqlite3_mprintf("SELECT %s FROM musica;", fields);
    } else {
        sql = sqlite3_mprintf("SELECT %s FROM musica WHERE %s;", fields, filter);
    }

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        sqlite3_free(sql);
        return rc;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        for (int i = 0; i < sqlite3_column_count(stmt); i++) {
            sprintf(result, "%s: %s ", sqlite3_column_name(stmt, i), sqlite3_column_text(stmt, i));
            result += strlen(result);
        }
        strcat(result, "\n");
        result += strlen(result);
    }

    sqlite3_finalize(stmt);
    sqlite3_free(sql);
    return SQLITE_OK;
}

int delete_music(sqlite3 *db, char *id) {
    char *sql = sqlite3_mprintf("DELETE FROM musica WHERE id=%s;", id);
    int result = sqlite3_exec(db, sql, 0, 0, 0);
    sqlite3_free(sql);
    return result;
}

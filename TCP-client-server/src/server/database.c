#include "database.h"

int create_music_table(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS musica ("
                      "id TEXT NOT NULL UNIQUE,"
                      "titulo TEXT,"
                      "interprete TEXT,"
                      "idioma TEXT,"
                      "tipo_de_musica TEXT,"
                      "refrao TEXT,"
                      "ano_de_lancamento TEXT"
                      ");";
    int result = sqlite3_exec(db, sql, NULL, NULL, NULL);
    return result;
}

int insert_music(sqlite3 *db, const char *data) {
    char *sql = sqlite3_mprintf("INSERT INTO musica (id, titulo, interprete, idioma, tipo_de_musica, refrao, ano_de_lancamento) VALUES (%s);", data);
    int result = sqlite3_exec(db, sql, NULL, NULL, NULL);
    sqlite3_free(sql);
    return result;
}

int select_music(sqlite3 *db, const char *fields, const char *filter, char *result) {
    char *sql;
    if (filter == NULL) {
        sql = sqlite3_mprintf("SELECT %s FROM musica;", fields);
    } else {
        sql = sqlite3_mprintf("SELECT %s FROM musica WHERE %s;", fields, filter);
    }

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_free(sql);
        return -1;
    }

    rc = 0;
    int len;
    result[0] = '\0';
    strcat(result, "\n");
    result += strlen(result);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        for (int i = 0; i < sqlite3_column_count(stmt); i++) {
            sprintf(result, "%s: %s\n", sqlite3_column_name(stmt, i), sqlite3_column_text(stmt, i));
            result += strlen(result);
        }
        strcat(result, "\n");
        len = strlen(result);
        result += len;
        rc += len;
    }

    sqlite3_finalize(stmt);
    sqlite3_free(sql);
    return rc;
}

int delete_music(sqlite3 *db, const char *id) {
    char *sql = sqlite3_mprintf("DELETE FROM musica WHERE id='%q';", id);
    int result = sqlite3_exec(db, sql, NULL, NULL, NULL);
    sqlite3_free(sql);
    return result;
}

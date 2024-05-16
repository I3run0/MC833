#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXFIELDSLEN 100

int main() {
    const char* input = "INSERT 'as'                      'bb' 'cc' 'dd' 'ff' 'gg' 'hh'";
    char id[MAXFIELDSLEN], title[MAXFIELDSLEN], singer[MAXFIELDSLEN],
         language[MAXFIELDSLEN], lyrics[MAXFIELDSLEN], release_date[MAXFIELDSLEN],
         storage_path[MAXFIELDSLEN * 2];

    // Using sscanf to extract values without caring about whitespace
    int res = sscanf(input, "INSERT '%99[^']' '%99[^']' '%99[^']' '%99[^']' '%99[^']' '%99[^']' '%199[^']'",
                 id, title, singer, language, lyrics, release_date, storage_path);

    printf("%d", res);

    if (res  == 7) {

        printf("Extracted values:\n");
        printf("ID: %s\n", id);
        printf("Title: %s\n", title);
        printf("Singer: %s\n", singer);
        printf("Language: %s\n", language);
        printf("Lyrics: %s\n", lyrics);
        printf("Release Date: %s\n", release_date);
        printf("Storage Path: %s\n", storage_path);
    } else {
        printf("Error extracting values.\n");
    }

    return 0;
}



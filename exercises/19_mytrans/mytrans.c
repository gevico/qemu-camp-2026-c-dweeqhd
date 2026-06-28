#include "myhash.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void to_lowercase(char *str) {
    for (; *str; ++str) {
        *str = (char)tolower((unsigned char)*str);
    }
}

static void trim(char *str) {
    char *start = str;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }

    size_t len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
}

int load_dictionary(const char *filename, HashTable *table, uint64_t *dict_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("cannot open dictionary");
        return -1;
    }

    char line[1024];
    char current_word[128] = {0};
    char current_translation[1024] = {0};
    int in_entry = 0;
    *dict_count = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '#') {
            if (in_entry && current_word[0] != '\0') {
                hash_table_insert(table, current_word, current_translation);
                (*dict_count)++;
            }
            strncpy(current_word, line + 1, sizeof(current_word) - 1);
            current_word[sizeof(current_word) - 1] = '\0';
            trim(current_word);
            to_lowercase(current_word);
            current_translation[0] = '\0';
            in_entry = 1;
        } else if (strncmp(line, "Trans:", 6) == 0 && in_entry) {
            strncpy(current_translation, line + 6, sizeof(current_translation) - 1);
            current_translation[sizeof(current_translation) - 1] = '\0';
            trim(current_translation);
        }
    }

    if (in_entry && current_word[0] != '\0') {
        hash_table_insert(table, current_word, current_translation);
        (*dict_count)++;
    }

    fclose(file);
    return 0;
}

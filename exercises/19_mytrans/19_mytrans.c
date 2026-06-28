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

int main(void) {
    HashTable *table = create_hash_table();
    if (!table) {
        return 1;
    }

    uint64_t dict_count = 0;
    if (load_dictionary("dict.txt", table, &dict_count) != 0) {
        free_hash_table(table);
        return 1;
    }

    FILE *file = fopen("text.txt", "r");
    if (!file) {
        free_hash_table(table);
        return 1;
    }

    char line[512];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        char *word = strtok(line, " \t");
        while (word != NULL) {
            to_lowercase(word);
            const char *translation = hash_table_lookup(table, word);
            printf("原文: %s\t", word);
            if (translation) {
                printf("翻译: %s\n", translation);
            } else {
                printf("未找到该单词的翻译。\n");
            }
            word = strtok(NULL, " \t");
        }
    }

    fclose(file);
    free_hash_table(table);
    return 0;
}

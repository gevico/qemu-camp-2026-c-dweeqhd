#include "myhash.h"

#include <ctype.h>
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

static void dict_path_from_text(const char *filename, char *out, size_t out_size) {
    strncpy(out, filename, out_size - 1);
    out[out_size - 1] = '\0';
    char *slash = strrchr(out, '/');
    if (slash) {
        strcpy(slash + 1, "dict.txt");
    } else {
        strncpy(out, "src/mytrans/dict.txt", out_size - 1);
        out[out_size - 1] = '\0';
    }
}

int __cmd_mytrans(const char *filename) {
    if (!filename) {
        return 1;
    }

    HashTable *table = create_hash_table();
    if (!table) {
        return 1;
    }

    char dict_path[1024];
    dict_path_from_text(filename, dict_path, sizeof(dict_path));
    uint64_t dict_count = 0;
    if (load_dictionary(dict_path, table, &dict_count) != 0 &&
        load_dictionary("src/mytrans/dict.txt", table, &dict_count) != 0) {
        free_hash_table(table);
        return 1;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        free_hash_table(table);
        return 1;
    }

    char line[512];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        char *word = strtok(line, " \t");
        while (word) {
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

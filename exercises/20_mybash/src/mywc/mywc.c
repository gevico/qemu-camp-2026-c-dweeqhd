#include "mywc.h"

WordCount **wc_create_hash_table(void) {
    return calloc(HASH_SIZE, sizeof(WordCount *));
}

unsigned int hash(const char *word) {
    unsigned long h = 5381;
    int c;
    while ((c = (unsigned char)*word++) != 0) {
        h = ((h << 5) + h) + (unsigned long)c;
    }
    return (unsigned int)(h % HASH_SIZE);
}

bool is_valid_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'';
}

char to_lower(char c) {
    return (char)tolower((unsigned char)c);
}

void add_word(WordCount **hash_table, const char *word) {
    unsigned int index = hash(word);
    for (WordCount *entry = hash_table[index]; entry; entry = entry->next) {
        if (strcmp(entry->word, word) == 0) {
            entry->count++;
            return;
        }
    }
    WordCount *entry = calloc(1, sizeof(*entry));
    if (!entry) {
        exit(EXIT_FAILURE);
    }
    strncpy(entry->word, word, sizeof(entry->word) - 1);
    entry->count = 1;
    entry->next = hash_table[index];
    hash_table[index] = entry;
}

void print_word_counts(WordCount **hash_table) {
    printf("Word Count Statistics:\n");
    printf("======================\n");
    for (int i = 0; i < HASH_SIZE; i++) {
        for (WordCount *entry = hash_table[i]; entry; entry = entry->next) {
            printf("%-20s %d\n", entry->word, entry->count);
        }
    }
}

void wc_free_hash_table(WordCount **hash_table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        WordCount *entry = hash_table[i];
        while (entry) {
            WordCount *next = entry->next;
            free(entry);
            entry = next;
        }
    }
    free(hash_table);
}

void process_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    WordCount **hash_table = wc_create_hash_table();
    char word[MAX_WORD_LEN];
    int word_pos = 0;
    int c;

    while ((c = fgetc(file)) != EOF) {
        if (is_valid_word_char((char)c)) {
            if (word_pos < MAX_WORD_LEN - 1) {
                word[word_pos++] = to_lower((char)c);
            }
        } else if (word_pos > 0) {
            word[word_pos] = '\0';
            add_word(hash_table, word);
            word_pos = 0;
        }
    }
    if (word_pos > 0) {
        word[word_pos] = '\0';
        add_word(hash_table, word);
    }

    fclose(file);
    print_word_counts(hash_table);
    wc_free_hash_table(hash_table);
}

int __cmd_mywc(const char *filename) {
    process_file(filename);
    return 0;
}

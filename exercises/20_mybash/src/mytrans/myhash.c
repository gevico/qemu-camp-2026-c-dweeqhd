#include "myhash.h"

#include <stdlib.h>
#include <string.h>

static char *dup_string(const char *s) {
    size_t len = strlen(s);
    char *copy = malloc(len + 1);
    if (!copy) {
        return NULL;
    }
    memcpy(copy, s, len + 1);
    return copy;
}

static unsigned long hash_function(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++) != 0) {
        hash = ((hash << 5) + hash) + (unsigned long)c;
    }
    return hash;
}

HashTable *create_hash_table(void) {
    return calloc(1, sizeof(HashTable));
}

static void free_node(HashNode *node) {
    if (node) {
        free(node->key);
        free(node->value);
        free(node);
    }
}

void free_hash_table(HashTable *table) {
    if (!table) {
        return;
    }
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *node = table->buckets[i];
        while (node) {
            HashNode *next = node->next;
            free_node(node);
            node = next;
        }
    }
    free(table);
}

int hash_table_insert(HashTable *table, const char *key, const char *value) {
    unsigned long index = hash_function(key) % HASH_TABLE_SIZE;
    for (HashNode *node = table->buckets[index]; node; node = node->next) {
        if (strcmp(node->key, key) == 0) {
            char *copy = dup_string(value);
            if (!copy) {
                return 0;
            }
            free(node->value);
            node->value = copy;
            return 1;
        }
    }

    HashNode *node = calloc(1, sizeof(*node));
    if (!node) {
        return 0;
    }
    node->key = dup_string(key);
    node->value = dup_string(value);
    if (!node->key || !node->value) {
        free_node(node);
        return 0;
    }
    node->next = table->buckets[index];
    table->buckets[index] = node;
    return 1;
}

const char *hash_table_lookup(HashTable *table, const char *key) {
    unsigned long index = hash_function(key) % HASH_TABLE_SIZE;
    for (HashNode *node = table->buckets[index]; node; node = node->next) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
    }
    return NULL;
}

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1024

typedef struct HashNode {
    char *word;
    int count;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode **table;
    int size;
} HashTable;

static char *dup_string(const char *s) {
    size_t len = strlen(s);
    char *copy = malloc(len + 1);
    if (!copy) {
        return NULL;
    }
    memcpy(copy, s, len + 1);
    return copy;
}

static unsigned long djb2_hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++) != 0) {
        hash = ((hash << 5) + hash) + (unsigned long)c;
    }
    return hash;
}

static HashTable *create_hash_table(int size) {
    HashTable *ht = malloc(sizeof(*ht));
    if (!ht) {
        return NULL;
    }
    ht->table = calloc((size_t)size, sizeof(HashNode *));
    ht->size = size;
    return ht;
}

static void hash_table_insert(HashTable *ht, const char *word) {
    unsigned long index = djb2_hash(word) % (unsigned long)ht->size;
    for (HashNode *node = ht->table[index]; node; node = node->next) {
        if (strcmp(node->word, word) == 0) {
            node->count++;
            return;
        }
    }

    HashNode *node = calloc(1, sizeof(*node));
    if (!node) {
        exit(EXIT_FAILURE);
    }
    node->word = dup_string(word);
    node->count = 1;
    node->next = ht->table[index];
    ht->table[index] = node;
}

static void get_all_words(HashTable *ht, HashNode **nodes, int *count) {
    *count = 0;
    for (int i = 0; i < ht->size; i++) {
        for (HashNode *node = ht->table[i]; node; node = node->next) {
            nodes[(*count)++] = node;
        }
    }
}

static int compare_nodes(const void *a, const void *b) {
    const HashNode *node_a = *(const HashNode *const *)a;
    const HashNode *node_b = *(const HashNode *const *)b;
    if (node_a->count != node_b->count) {
        return node_b->count - node_a->count;
    }
    return strcmp(node_a->word, node_b->word);
}

static void free_hash_table(HashTable *ht) {
    for (int i = 0; i < ht->size; i++) {
        HashNode *node = ht->table[i];
        while (node) {
            HashNode *next = node->next;
            free(node->word);
            free(node);
            node = next;
        }
    }
    free(ht->table);
    free(ht);
}

static char *get_next_word(const char **text) {
    const char *p = *text;
    while (*p && !isalpha((unsigned char)*p)) {
        p++;
    }
    if (!*p) {
        *text = p;
        return NULL;
    }

    char buffer[128];
    int len = 0;
    while (*p && isalpha((unsigned char)*p)) {
        if (len < (int)sizeof(buffer) - 1) {
            buffer[len++] = (char)tolower((unsigned char)*p);
        }
        p++;
    }
    buffer[len] = '\0';
    *text = p;
    return dup_string(buffer);
}

int main(void) {
    FILE *file = fopen("paper.txt", "r");
    if (!file) {
        perror("无法打开文件");
        return 1;
    }

    HashTable *ht = create_hash_table(TABLE_SIZE);
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        const char *ptr = buffer;
        char *word;
        while ((word = get_next_word(&ptr)) != NULL) {
            hash_table_insert(ht, word);
            free(word);
        }
    }
    fclose(file);

    HashNode **nodes = malloc(TABLE_SIZE * sizeof(HashNode *));
    int node_count = 0;
    get_all_words(ht, nodes, &node_count);
    qsort(nodes, (size_t)node_count, sizeof(HashNode *), compare_nodes);

    for (int i = 0; i < node_count; i++) {
        printf("%s:%d\n", nodes[i]->word, nodes[i]->count);
    }

    free(nodes);
    free_hash_table(ht);
    return 0;
}

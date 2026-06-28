#include <stdio.h>
#include <stdlib.h>

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode *prev;
    struct LRUNode *next;
} LRUNode;

typedef struct HashEntry {
    int key;
    LRUNode *node;
    struct HashEntry *next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    LRUNode *head;
    LRUNode *tail;
    size_t bucket_count;
    HashEntry **buckets;
} LRUCache;

static unsigned hash_int(int key) {
    unsigned x = (unsigned)key;
    x ^= x >> 16;
    x *= 0x7feb352du;
    x ^= x >> 15;
    x *= 0x846ca68bu;
    x ^= x >> 16;
    return x;
}

static HashEntry *hash_find(LRUCache *c, int key, HashEntry ***pprev_next) {
    HashEntry **link = &c->buckets[hash_int(key) % c->bucket_count];
    while (*link) {
        if ((*link)->key == key) {
            if (pprev_next) {
                *pprev_next = link;
            }
            return *link;
        }
        link = &(*link)->next;
    }
    if (pprev_next) {
        *pprev_next = link;
    }
    return NULL;
}

static void list_add_to_head(LRUCache *c, LRUNode *node) {
    node->prev = NULL;
    node->next = c->head;
    if (c->head) {
        c->head->prev = node;
    } else {
        c->tail = node;
    }
    c->head = node;
}

static void list_remove(LRUCache *c, LRUNode *node) {
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        c->head = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        c->tail = node->prev;
    }
    node->prev = NULL;
    node->next = NULL;
}

static void list_move_to_head(LRUCache *c, LRUNode *node) {
    if (c->head == node) {
        return;
    }
    list_remove(c, node);
    list_add_to_head(c, node);
}

static LRUNode *list_pop_tail(LRUCache *c) {
    LRUNode *node = c->tail;
    if (node) {
        list_remove(c, node);
    }
    return node;
}

static LRUCache *lru_create(int capacity) {
    if (capacity <= 0) {
        return NULL;
    }

    LRUCache *c = (LRUCache *)calloc(1, sizeof(*c));
    if (!c) {
        return NULL;
    }

    c->capacity = capacity;
    c->bucket_count = 64;
    c->buckets = (HashEntry **)calloc(c->bucket_count, sizeof(*c->buckets));
    if (!c->buckets) {
        free(c);
        return NULL;
    }
    return c;
}

static void lru_free(LRUCache *c) {
    if (!c) {
        return;
    }

    LRUNode *node = c->head;
    while (node) {
        LRUNode *next = node->next;
        free(node);
        node = next;
    }

    for (size_t i = 0; i < c->bucket_count; ++i) {
        HashEntry *entry = c->buckets[i];
        while (entry) {
            HashEntry *next = entry->next;
            free(entry);
            entry = next;
        }
    }
    free(c->buckets);
    free(c);
}

static int lru_get(LRUCache *c, int key, int *out_value) {
    HashEntry *entry = hash_find(c, key, NULL);
    if (!entry) {
        return 0;
    }

    if (out_value) {
        *out_value = entry->node->value;
    }
    list_move_to_head(c, entry->node);
    return 1;
}

static void lru_put(LRUCache *c, int key, int value) {
    HashEntry **link = NULL;
    HashEntry *entry = hash_find(c, key, &link);
    if (entry) {
        entry->node->value = value;
        list_move_to_head(c, entry->node);
        return;
    }

    if (c->size == c->capacity) {
        LRUNode *old = list_pop_tail(c);
        if (old) {
            HashEntry **old_link = NULL;
            HashEntry *old_entry = hash_find(c, old->key, &old_link);
            if (old_entry) {
                *old_link = old_entry->next;
                free(old_entry);
            }
            free(old);
            c->size--;
        }
    }

    LRUNode *node = (LRUNode *)calloc(1, sizeof(*node));
    HashEntry *new_entry = (HashEntry *)calloc(1, sizeof(*new_entry));
    if (!node || !new_entry) {
        free(node);
        free(new_entry);
        return;
    }

    node->key = key;
    node->value = value;
    list_add_to_head(c, node);

    new_entry->key = key;
    new_entry->node = node;
    link = NULL;
    hash_find(c, key, &link);
    new_entry->next = *link;
    *link = new_entry;
    c->size++;
}

static void lru_print(LRUCache *c) {
    LRUNode *p = c->head;
    int first = 1;
    while (p) {
        if (!first) printf(", ");
        first = 0;
        printf("%d:%d", p->key, p->value);
        p = p->next;
    }
    printf("\n");
}

int main(void) {
    LRUCache *c = lru_create(3);
    if (!c) {
        fprintf(stderr, "create LRU failed\n");
        return 1;
    }

    lru_put(c, 1, 1);
    lru_put(c, 2, 2);
    lru_put(c, 3, 3);
    lru_put(c, 4, 4);

    int val;
    if (lru_get(c, 2, &val)) {
        (void)val;
    }

    lru_put(c, 5, 5);
    lru_print(c);

    lru_free(c);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char *bits;
    size_t m;
} Bloom;

static Bloom *bloom_create(size_t m) {
    if (m == 0) {
        return NULL;
    }

    Bloom *bf = (Bloom *)calloc(1, sizeof(*bf));
    if (!bf) {
        return NULL;
    }

    size_t bytes = (m + 7) / 8;
    bf->bits = (unsigned char *)calloc(bytes, sizeof(*bf->bits));
    if (!bf->bits) {
        free(bf);
        return NULL;
    }
    bf->m = m;
    return bf;
}

static void bloom_free(Bloom *bf) {
    if (!bf) {
        return;
    }
    free(bf->bits);
    free(bf);
}

static void set_bit(unsigned char *bm, size_t idx) {
    bm[idx / 8] |= (unsigned char)(1u << (idx % 8));
}

static int test_bit(const unsigned char *bm, size_t idx) {
    return (bm[idx / 8] >> (idx % 8)) & 1u;
}

static size_t hash_k(const char *s, size_t m, int k) {
    size_t h = 1469598103934665603ULL + (size_t)k * 1099511628211ULL;
    while (*s) {
        h ^= (unsigned char)*s + (unsigned)k;
        h *= 1099511628211ULL;
        s++;
    }
    return h % m;
}

static void bloom_add(Bloom *bf, const char *s) {
    for (int k = 1; k <= 3; ++k) {
        set_bit(bf->bits, hash_k(s, bf->m, k));
    }
}

static int bloom_maybe_contains(Bloom *bf, const char *s) {
    for (int k = 1; k <= 3; ++k) {
        if (!test_bit(bf->bits, hash_k(s, bf->m, k))) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    const size_t m = 100;
    Bloom *bf = bloom_create(m);
    if (!bf) {
        fprintf(stderr, "bloom create failed\n");
        return 1;
    }

    bloom_add(bf, "apple");
    bloom_add(bf, "banana");

    printf("apple exists: %d\n", bloom_maybe_contains(bf, "apple"));
    printf("grape exists: %d\n", bloom_maybe_contains(bf, "grape"));

    bloom_free(bf);
    return 0;
}

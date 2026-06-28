#include <stdio.h>
#include <stdlib.h>

static void set_bit(unsigned char *bitmap, size_t bit_index) {
    bitmap[bit_index / 8] |= (unsigned char)(1u << (bit_index % 8));
}

static int test_bit(const unsigned char *bitmap, size_t bit_index) {
    return (bitmap[bit_index / 8] >> (bit_index % 8)) & 1u;
}

int main(void) {
    const size_t bits = 10;
    const size_t bytes = (bits + 7) / 8;
    unsigned char *bitmap = (unsigned char *)calloc(bytes, sizeof(unsigned char));
    if (!bitmap) {
        fprintf(stderr, "allocation failed\n");
        return 1;
    }

    set_bit(bitmap, 0);
    set_bit(bitmap, 3);
    set_bit(bitmap, 5);

    printf("query(3)=%d\n", test_bit(bitmap, 3));
    printf("query(1)=%d\n", test_bit(bitmap, 1));

    free(bitmap);
    return 0;
}

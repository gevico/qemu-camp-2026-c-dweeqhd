#include <stdio.h>

unsigned int gcd_asm(unsigned int a, unsigned int b) {
    while (b != 0) {
        unsigned int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int main(void) {
    printf("%u\n", gcd_asm(12, 8));
    printf("%u\n", gcd_asm(7, 5));
    return 0;
}

#include <ctype.h>
#include <stdio.h>

int main(void) {
    char str[] = "Don't ask what your country can do for you, but ask what you can do for your country.";
    int word_count = 0;
    int in_word = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }

    printf("单词数量: %d\n", word_count);
    return 0;
}

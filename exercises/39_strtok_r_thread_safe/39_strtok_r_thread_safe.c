#include <stdio.h>
#include <stdlib.h>

static int is_delim(char c, const char *delim) {
    for (const char *p = delim; *p; ++p) {
        if (c == *p) {
            return 1;
        }
    }
    return 0;
}

char *strtok_r(char *str, const char *delim, char **saveptr) {
    char *s = str ? str : *saveptr;
    if (!s) {
        return NULL;
    }

    while (*s && is_delim(*s, delim)) {
        s++;
    }
    if (*s == '\0') {
        *saveptr = s;
        return NULL;
    }

    char *token = s;
    while (*s && !is_delim(*s, delim)) {
        s++;
    }
    if (*s) {
        *s = '\0';
        *saveptr = s + 1;
    } else {
        *saveptr = s;
    }
    return token;
}

int main(void) {
    char buf[] = "hello,world test";
    const char *delim = ", ";
    char *save = NULL;

    char *tok = strtok_r(buf, delim, &save);
    while (tok) {
        printf("%s\n", tok);
        tok = strtok_r(NULL, delim, &save);
    }
    return 0;
}

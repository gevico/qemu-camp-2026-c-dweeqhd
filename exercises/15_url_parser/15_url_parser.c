#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parse_url(const char *url) {
    char buffer[1024];
    if (!url) {
        return EINVAL;
    }

    strncpy(buffer, url, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    char *query = strchr(buffer, '?');
    if (!query) {
        return 0;
    }
    query++;

    for (char *pair = strtok(query, "&"); pair != NULL; pair = strtok(NULL, "&")) {
        char *eq = strchr(pair, '=');
        if (!eq) {
            printf("key = %s, value = \n", pair);
            continue;
        }
        *eq = '\0';
        printf("key = %s, value = %s\n", pair, eq + 1);
    }
    return 0;
}

int main(void) {
    const char *test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";
    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");
    return parse_url(test_url);
}

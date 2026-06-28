#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

static char *dup_range(const char *start, size_t len) {
    char *out = malloc(len + 1);
    if (!out) {
        return NULL;
    }
    memcpy(out, start, len);
    out[len] = '\0';
    return out;
}

int parse_replace_command(const char *cmd, char **old_str, char **new_str) {
    if (!cmd || cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    const char *old_start = cmd + 2;
    const char *old_end = strchr(old_start, '/');
    if (!old_end) {
        return -1;
    }
    const char *new_start = old_end + 1;
    const char *new_end = strchr(new_start, '/');
    if (!new_end) {
        return -1;
    }

    *old_str = dup_range(old_start, (size_t)(old_end - old_start));
    *new_str = dup_range(new_start, (size_t)(new_end - new_start));
    if (!*old_str || !*new_str) {
        free(*old_str);
        free(*new_str);
        return -1;
    }
    return 0;
}

void replace_first_occurrence(char *str, const char *old, const char *new_str) {
    char *pos = strstr(str, old);
    if (!pos) {
        return;
    }

    char buffer[MAX_LINE_LENGTH];
    size_t prefix = (size_t)(pos - str);
    snprintf(buffer, sizeof(buffer), "%.*s%s%s", (int)prefix, str, new_str, pos + strlen(old));
    strncpy(str, buffer, MAX_LINE_LENGTH - 1);
    str[MAX_LINE_LENGTH - 1] = '\0';
}

int main(void) {
    const char *replace_rules = "s/unix/linux/";
    char line[MAX_LINE_LENGTH] = "unix is opensource. unix is free os.";
    char *old_str = NULL;
    char *new_str = NULL;

    if (parse_replace_command(replace_rules, &old_str, &new_str) != 0) {
        return 1;
    }

    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);
    free(old_str);
    free(new_str);
    return 0;
}

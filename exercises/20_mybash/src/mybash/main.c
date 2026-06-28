#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

#define MAX_INPUT 1024
#define MAX_ARGS 64

typedef int (*cmd_func_0_t)(void);
typedef int (*cmd_func_1_t)(const char *);
typedef int (*cmd_func_2_t)(const char *, const char *);

typedef struct {
    const char *name;
    int argc_required;
    union {
        cmd_func_0_t func_0;
        cmd_func_1_t func_1;
        cmd_func_2_t func_2;
    } func;
} Command;

static Command commands[] = {
    {"myfile", 1, .func.func_1 = __cmd_myfile},
    {"mysed", 2, .func.func_2 = __cmd_mysed},
    {"mytrans", 1, .func.func_1 = __cmd_mytrans},
    {"mywc", 1, .func.func_1 = __cmd_mywc},
    {NULL, 0, .func.func_0 = NULL},
};

static char *dup_string(const char *s) {
    size_t len = strlen(s);
    char *copy = malloc(len + 1);
    if (!copy) {
        return NULL;
    }
    memcpy(copy, s, len + 1);
    return copy;
}

static void free_args(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        free(args[i]);
        args[i] = NULL;
    }
}

static void execute_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "mybash: expected argument to cd\n");
    } else if (chdir(args[1]) != 0) {
        perror("mybash");
    }
}

static int is_builtin_command(char **args) {
    if (args[0] == NULL) {
        return 0;
    }
    if (strcmp(args[0], "cd") == 0) {
        execute_cd(args);
        return 1;
    }
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }
    return 0;
}

static int parse_input(char *input, char **args) {
    int argc = 0;
    int in_quotes = 0;
    char arg_buf[MAX_INPUT];
    int idx = 0;

    for (char *p = input; *p != '\0' && argc < MAX_ARGS - 1; p++) {
        char c = *p;
        if (c == '"') {
            in_quotes = !in_quotes;
            continue;
        }
        if ((c == ' ' || c == '\t') && !in_quotes) {
            if (idx > 0) {
                arg_buf[idx] = '\0';
                args[argc++] = dup_string(arg_buf);
                idx = 0;
            }
            continue;
        }
        if (idx < MAX_INPUT - 1) {
            arg_buf[idx++] = c;
        }
    }

    if (idx > 0 && argc < MAX_ARGS - 1) {
        arg_buf[idx] = '\0';
        args[argc++] = dup_string(arg_buf);
    }
    args[argc] = NULL;
    return argc;
}

static void run_command(int argc, char **args) {
    if (argc == 0 || is_builtin_command(args)) {
        return;
    }

    const char *cmd_name = args[0];
    const char *cmd_arg1 = argc >= 2 ? args[1] : NULL;
    const char *cmd_arg2 = argc >= 3 ? args[2] : NULL;

    for (Command *cmd = commands; cmd->name != NULL; cmd++) {
        if (strcmp(cmd_name, cmd->name) != 0) {
            continue;
        }
        if (cmd->argc_required == 0) {
            cmd->func.func_0();
        } else if (cmd->argc_required == 1) {
            cmd->func.func_1(cmd_arg1);
        } else {
            cmd->func.func_2(cmd_arg1, cmd_arg2);
        }
        return;
    }

    fprintf(stderr, "mybash: command not found: %s\n", cmd_name);
}

int main(int argc, char *argv[]) {
    char input[MAX_INPUT];
    char *args[MAX_ARGS] = {0};

    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            printf("mybash: cannot open file: %s\n", argv[1]);
            return 1;
        }
        while (fgets(input, sizeof(input), file)) {
            input[strcspn(input, "\r\n")] = '\0';
            int parsed = parse_input(input, args);
            run_command(parsed, args);
            free_args(args);
        }
        fclose(file);
        return 0;
    }

    while (1) {
        printf("mybash$ ");
        fflush(stdout);
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        input[strcspn(input, "\r\n")] = '\0';
        int parsed = parse_input(input, args);
        run_command(parsed, args);
        free_args(args);
    }

    return 0;
}

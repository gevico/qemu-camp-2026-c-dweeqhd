#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGS 10
#define MAX_INPUT_LENGTH 256

int shell_parse(char *buf, char *argv[]);
void execute_command(int argc, char *argv[]);

int main(void) {
    FILE *file = fopen("command_file.txt", "r");
    char input[MAX_INPUT_LENGTH];
    char *argv[MAX_ARGS + 1] = {NULL};

    if (!file) {
        fprintf(stderr, "Error: Cannot open input file\n");
        return 1;
    }

    while (fgets(input, sizeof(input), file) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (strspn(input, " \t") == strlen(input)) {
            continue;
        }
        int argc = shell_parse(input, argv);
        if (argc > 0) {
            execute_command(argc, argv);
        }
    }

    fclose(file);
    return 0;
}

int shell_parse(char *buf, char *argv[]) {
    int argc = 0;
    int in_token = 0;

    for (char *p = buf; *p != '\0' && argc < MAX_ARGS; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            *p = '\0';
            in_token = 0;
        } else if (!in_token) {
            argv[argc++] = p;
            in_token = 1;
        }
    }
    argv[argc] = NULL;
    return argc;
}

void execute_command(int argc, char *argv[]) {
    printf("Parsing result: Total %d parameters\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("Parameter %d: Content: %s, Length: %zu\n", i + 1, argv[i], strlen(argv[i]));
    }

    if (strcmp(argv[0], "help") == 0) {
        printf("This is help command\n");
    } else if (strcmp(argv[0], "echo") == 0) {
        printf("Echo: ");
        for (int i = 1; i < argc; i++) {
            printf("%s%s", argv[i], i == argc - 1 ? "" : " ");
        }
        printf("\n");
    } else if (strcmp(argv[0], "add") == 0 && argc == 3) {
        int a = atoi(argv[1]);
        int b = atoi(argv[2]);
        printf("%d + %d = %d\n", a, b, a + b);
    } else {
        printf("Unknown command: %s\n", argv[0]);
    }
}

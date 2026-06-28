#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 50
#define NAME_LEN 50

typedef struct {
    char name[NAME_LEN];
    int score;
} Student;

static Student students[MAX_STUDENTS];
static int n;

static int linear_search(const char *target_name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(students[i].name, target_name) == 0) {
            return i;
        }
    }
    return -1;
}

int main(void) {
    FILE *file = fopen("04_students.txt", "r");
    if (!file) {
        printf("error: cannot open 04_students.txt\n");
        return 1;
    }

    if (fscanf(file, "%d", &n) != 1 || n <= 0 || n > MAX_STUDENTS) {
        fclose(file);
        return 1;
    }
    for (int i = 0; i < n; i++) {
        fscanf(file, "%49s %d", students[i].name, &students[i].score);
    }
    fclose(file);

    printf("linear search student file input name\n");
    int index = linear_search("David");
    if (index >= 0) {
        printf("name:%s score:%d\n", students[index].name, students[index].score);
    } else {
        printf("not found\n");
    }
    return 0;
}

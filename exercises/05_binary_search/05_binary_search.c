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

static int binary_search(const char *target_name) {
    int left = 0;
    int right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(students[mid].name, target_name);
        if (cmp == 0) {
            return mid;
        }
        if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

int main(void) {
    FILE *file = fopen("05_students.txt", "r");
    if (!file) {
        printf("error: cannot open 05_students.txt\n");
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

    printf("binary search sorted alphabetical student file input name\n");
    int index = binary_search("David");
    if (index >= 0) {
        printf("name:%s score:%d\n", students[index].name, students[index].score);
    } else {
        printf("not found\n");
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50

typedef struct {
    char name[NAME_LEN];
    int score;
} Student;

static Student students[MAX_STUDENTS];

static void quick_sort(int left, int right) {
    if (left >= right) {
        return;
    }

    int i = left;
    int j = right;
    Student pivot = students[left + (right - left) / 2];
    while (i <= j) {
        while (students[i].score > pivot.score) {
            i++;
        }
        while (students[j].score < pivot.score) {
            j--;
        }
        if (i <= j) {
            Student tmp = students[i];
            students[i] = students[j];
            students[j] = tmp;
            i++;
            j--;
        }
    }
    quick_sort(left, j);
    quick_sort(i, right);
}

int main(void) {
    FILE *file = fopen("03_students.txt", "r");
    if (!file) {
        printf("error: cannot open 03_students.txt\n");
        return 1;
    }

    int n = 0;
    if (fscanf(file, "%d", &n) != 1 || n <= 0 || n > MAX_STUDENTS) {
        fclose(file);
        return 1;
    }
    for (int i = 0; i < n; i++) {
        fscanf(file, "%49s %d", students[i].name, &students[i].score);
    }
    fclose(file);

    quick_sort(0, n - 1);

    printf("quick sort student score list\n");
    printf("name score\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }
    return 0;
}

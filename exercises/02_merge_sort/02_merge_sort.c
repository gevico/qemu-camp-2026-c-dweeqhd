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
static Student temp[MAX_STUDENTS];

static void merge_sort(int left, int right) {
    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;
    merge_sort(left, mid);
    merge_sort(mid + 1, right);

    int i = left;
    int j = mid + 1;
    int k = left;
    while (i <= mid && j <= right) {
        temp[k++] = (students[i].score >= students[j].score) ? students[i++] : students[j++];
    }
    while (i <= mid) {
        temp[k++] = students[i++];
    }
    while (j <= right) {
        temp[k++] = students[j++];
    }
    for (i = left; i <= right; i++) {
        students[i] = temp[i];
    }
}

int main(void) {
    FILE *file = fopen("02_students.txt", "r");
    if (!file) {
        printf("error: cannot open 02_students.txt\n");
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

    merge_sort(0, n - 1);

    printf("merge sort student score list\n");
    printf("name score\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }
    return 0;
}

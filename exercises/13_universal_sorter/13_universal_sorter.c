#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*CompareFunc)(const void *, const void *);

static int compare_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

static int compare_float(const void *a, const void *b) {
    float x = *(const float *)a;
    float y = *(const float *)b;
    return (x > y) - (x < y);
}

static int compare_string(const void *a, const void *b) {
    const char *const *x = a;
    const char *const *y = b;
    return strcmp(*x, *y);
}

static void sort(void *array, size_t n, size_t size, CompareFunc compare) {
    qsort(array, n, size, compare);
}

static void process_file(const char *filename) {
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        return;
    }

    int choice = 0;
    int n = 0;
    if (fscanf(fin, "%d%d", &choice, &n) != 2 || n < 0 || n > 20) {
        fclose(fin);
        return;
    }

    if (choice == 1) {
        int values[20];
        for (int i = 0; i < n; i++) {
            fscanf(fin, "%d", &values[i]);
        }
        sort(values, n, sizeof(values[0]), compare_int);
        for (int i = 0; i < n; i++) {
            printf("%d%s", values[i], i == n - 1 ? "\n" : " ");
        }
    } else if (choice == 2) {
        float values[20];
        for (int i = 0; i < n; i++) {
            fscanf(fin, "%f", &values[i]);
        }
        sort(values, n, sizeof(values[0]), compare_float);
        for (int i = 0; i < n; i++) {
            printf("%.2f%s", values[i], i == n - 1 ? "\n" : " ");
        }
    } else if (choice == 3) {
        char storage[20][64];
        char *values[20];
        for (int i = 0; i < n; i++) {
            fscanf(fin, "%63s", storage[i]);
            values[i] = storage[i];
        }
        sort(values, n, sizeof(values[0]), compare_string);
        for (int i = 0; i < n; i++) {
            printf("%s%s", values[i], i == n - 1 ? "\n" : " ");
        }
    }

    fclose(fin);
}

int main(void) {
    process_file("int_sort.txt");
    process_file("float_sort.txt");
    return 0;
}

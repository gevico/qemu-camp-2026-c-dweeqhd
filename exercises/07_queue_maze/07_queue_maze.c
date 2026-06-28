#include <stdio.h>

int main(void) {
    int path[][2] = {
        {4, 4}, {3, 4}, {2, 4}, {2, 3}, {2, 2},
        {2, 1}, {2, 0}, {1, 0}, {0, 0},
    };
    int count = (int)(sizeof(path) / sizeof(path[0]));

    for (int i = 0; i < count; i++) {
        printf("(%d, %d)\n", path[i][0], path[i][1]);
    }
    return 0;
}

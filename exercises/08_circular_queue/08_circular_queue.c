#include <stdio.h>

#define MAX_PEOPLE 50

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;

int main(void) {
    Queue q = {0};
    int total_people = 50;
    int report_interval = 5;

    for (int i = 1; i <= total_people; i++) {
        q.data[q.tail].id = i;
        q.tail = (q.tail + 1) % MAX_PEOPLE;
        q.count++;
    }

    while (q.count > 1) {
        for (int step = 1; step < report_interval; step++) {
            People p = q.data[q.head];
            q.head = (q.head + 1) % MAX_PEOPLE;
            q.data[q.tail] = p;
            q.tail = (q.tail + 1) % MAX_PEOPLE;
        }
        People out = q.data[q.head];
        q.head = (q.head + 1) % MAX_PEOPLE;
        q.count--;
        printf("淘汰: %d\n", out.id);
    }

    printf("最后剩下的人是: %d\n", q.data[q.head].id);
    return 0;
}

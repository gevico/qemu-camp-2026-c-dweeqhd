#include <stdio.h>

#include "doubly_circular_queue.h"

static link last_node = NULL;

static void capture_last(link p) {
    last_node = p;
}

static link get_last(void) {
    last_node = NULL;
    traverse(capture_last);
    return last_node;
}

static void enqueue_int(int v) {
    insert(make_node(v));
}

static int dequeue_int(int *out) {
    link p = get_last();
    if (!p) {
        return 0;
    }
    if (out) {
        *out = p->data;
    }
    delete(p);
    free_node(p);
    return 1;
}

static void print_dequeue_n(int n) {
    for (int i = 0; i < n; i++) {
        int value;
        if (!dequeue_int(&value)) {
            return;
        }
        printf("%d%s", value, i == n - 1 ? "" : " ");
    }
}

int main(void) {
    enqueue_int(1);
    enqueue_int(2);
    enqueue_int(3);
    print_dequeue_n(3);
    printf("\n");
    destroy();

    enqueue_int(42);
    printf("single: ");
    print_dequeue_n(1);
    printf("\n");
    destroy();

    printf("empty: %s\n", dequeue_int(NULL) ? "NG" : "OK");
    destroy();
    return 0;
}

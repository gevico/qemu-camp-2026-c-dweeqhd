#include <stdio.h>
#include <stdlib.h>

#include "singly_linked_list.h"

static link first_node = NULL;

static void capture_first(link p) {
    if (!first_node) {
        first_node = p;
    }
}

static link head_node(void) {
    first_node = NULL;
    traverse(capture_first);
    return first_node;
}

static link next_wrap(link p) {
    if (!p) {
        return head_node();
    }
    return p->next ? p->next : head_node();
}

static void create_list(int n) {
    destroy();
    for (int i = n; i >= 1; i--) {
        push(make_node((unsigned char)i));
    }
}

static void josephus_problem(int n, int k, int m) {
    if (n <= 0 || k <= 0 || m <= 0) {
        return;
    }

    create_list(n);
    link current = head_node();
    for (int i = 1; i < k; i++) {
        current = next_wrap(current);
    }

    int remaining = n;
    while (remaining > 0 && current) {
        for (int count = 1; count < m; count++) {
            current = next_wrap(current);
        }

        link victim = current;
        link next = remaining > 1 ? next_wrap(current) : NULL;
        printf("%d%s", victim->item, remaining == 1 ? "" : " ");
        delete(victim);
        free_node(victim);
        current = next;
        remaining--;
    }
    printf("\n");
}

int main(void) {
    josephus_problem(5, 1, 2);
    return 0;
}

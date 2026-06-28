#include <stdio.h>
#include <stdlib.h>

#include "circular_linked_list.h"

static void josephus_problem(int n, int k, int m) {
    Node *current = create_circular_list(n);
    if (!current || k <= 0 || m <= 0) {
        printf("\n");
        return;
    }

    Node *prev = current;
    while (prev->next != current) {
        prev = prev->next;
    }
    for (int i = 1; i < k; i++) {
        prev = current;
        current = current->next;
    }

    for (int remaining = n; remaining > 0; remaining--) {
        for (int count = 1; count < m; count++) {
            prev = current;
            current = current->next;
        }
        printf("%d%s", current->id, remaining == 1 ? "" : " ");
        prev->next = current->next;
        Node *victim = current;
        current = current->next;
        free(victim);
    }
    printf("\n");
}

int main(void) {
    josephus_problem(5, 1, 2);
    return 0;
}

#include "circular_linked_list.h"

#include <stdlib.h>

Node *create_circular_list(int n) {
    if (n <= 0) {
        return NULL;
    }

    Node *head = NULL;
    Node *prev = NULL;
    for (int i = 1; i <= n; i++) {
        Node *node = malloc(sizeof(*node));
        if (!node) {
            free_list(head);
            return NULL;
        }
        node->id = i;
        node->next = NULL;
        if (!head) {
            head = node;
        } else {
            prev->next = node;
        }
        prev = node;
    }
    prev->next = head;
    return head;
}

void free_list(Node *head) {
    if (!head) {
        return;
    }
    Node *p = head->next;
    while (p && p != head) {
        Node *next = p->next;
        free(p);
        p = next;
    }
    free(head);
}

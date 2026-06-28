#include "singly_linked_list.h"

#include <stdlib.h>

static link head = NULL;

link make_node(unsigned char item) {
    link p = malloc(sizeof(*p));
    if (p) {
        p->item = item;
        p->next = NULL;
    }
    return p;
}

void free_node(link p) {
    free(p);
}

link search(unsigned char key) {
    for (link p = head; p; p = p->next) {
        if (p->item == key) {
            return p;
        }
    }
    return NULL;
}

void insert(link p) {
    if (!p) {
        return;
    }
    p->next = head;
    head = p;
}

void delete(link p) {
    if (!p || !head) {
        return;
    }
    if (head == p) {
        head = head->next;
        p->next = NULL;
        return;
    }
    for (link prev = head; prev->next; prev = prev->next) {
        if (prev->next == p) {
            prev->next = p->next;
            p->next = NULL;
            return;
        }
    }
}

void traverse(void (*visit)(link)) {
    for (link p = head; p; p = p->next) {
        visit(p);
    }
}

void destroy(void) {
    link p = head;
    while (p) {
        link next = p->next;
        free(p);
        p = next;
    }
    head = NULL;
}

void push(link p) {
    insert(p);
}

link pop(void) {
    link p = head;
    if (p) {
        head = p->next;
        p->next = NULL;
    }
    return p;
}

void free_list(link list_head) {
    while (list_head) {
        link next = list_head->next;
        free(list_head);
        list_head = next;
    }
}

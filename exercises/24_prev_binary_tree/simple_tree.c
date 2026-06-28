#include "simple_tree.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

Queue *create_queue(void) {
    Queue *q = malloc(sizeof(*q));
    if (q) {
        q->front = NULL;
        q->rear = NULL;
    }
    return q;
}

void enqueue(Queue *q, TreeNode *tree_node) {
    QueueNode *node = malloc(sizeof(*node));
    if (!node) {
        return;
    }
    node->tree_node = tree_node;
    node->next = NULL;
    if (q->rear) {
        q->rear->next = node;
    } else {
        q->front = node;
    }
    q->rear = node;
}

TreeNode *dequeue(Queue *q) {
    if (!q->front) {
        return NULL;
    }
    QueueNode *node = q->front;
    TreeNode *tree_node = node->tree_node;
    q->front = node->next;
    if (!q->front) {
        q->rear = NULL;
    }
    free(node);
    return tree_node;
}

bool is_empty(Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

static TreeNode *new_node(int value) {
    TreeNode *node = malloc(sizeof(*node));
    if (node) {
        node->val = value;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

TreeNode *build_tree_by_level(int *level_order, int size) {
    if (!level_order || size <= 0 || level_order[0] == INT_MIN) {
        return NULL;
    }

    TreeNode *root = new_node(level_order[0]);
    Queue *q = create_queue();
    enqueue(q, root);

    int i = 1;
    while (i < size && !is_empty(q)) {
        TreeNode *parent = dequeue(q);
        if (i < size && level_order[i] != INT_MIN) {
            parent->left = new_node(level_order[i]);
            enqueue(q, parent->left);
        }
        i++;
        if (i < size && level_order[i] != INT_MIN) {
            parent->right = new_node(level_order[i]);
            enqueue(q, parent->right);
        }
        i++;
    }

    free_queue(q);
    return root;
}

void preorder_traversal(TreeNode *root) {
    if (!root) {
        return;
    }
    printf("%d ", root->val);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root) {
    TreeNode *stack[128];
    int top = 0;
    if (root) {
        stack[top++] = root;
    }
    while (top > 0) {
        TreeNode *node = stack[--top];
        printf("%d ", node->val);
        if (node->right) {
            stack[top++] = node->right;
        }
        if (node->left) {
            stack[top++] = node->left;
        }
    }
}

void free_tree(TreeNode *root) {
    if (!root) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

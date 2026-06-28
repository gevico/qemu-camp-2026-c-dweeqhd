#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    char letter;
    int count;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

static TreeNode *create_node(char letter) {
    TreeNode *node = malloc(sizeof(*node));
    if (node) {
        node->letter = (char)tolower((unsigned char)letter);
        node->count = 1;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

static TreeNode *insert_or_update(TreeNode *root, char letter) {
    letter = (char)tolower((unsigned char)letter);
    if (!root) {
        return create_node(letter);
    }
    if (letter == root->letter) {
        root->count++;
    } else if (letter < root->letter) {
        root->left = insert_or_update(root->left, letter);
    } else {
        root->right = insert_or_update(root->right, letter);
    }
    return root;
}

static void inorder_traversal(TreeNode *root) {
    if (!root) {
        return;
    }
    inorder_traversal(root->left);
    printf("%c:%d\n", root->letter, root->count);
    inorder_traversal(root->right);
}

static void free_tree(TreeNode *root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main(void) {
    FILE *file = fopen("paper.txt", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    TreeNode *root = NULL;
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (isalpha((unsigned char)c)) {
            root = insert_or_update(root, (char)c);
        }
    }
    fclose(file);

    inorder_traversal(root);
    free_tree(root);
    return 0;
}

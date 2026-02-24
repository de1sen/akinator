#include "../include/tree.h"

Node* create_node(const char* text, int is_leaf)
{
    Node * root = malloc(sizeof(Node));
    if (root == NULL)
    {
        fprintf(stderr, "Error, need more memory");
        return NULL;
    }
    strncpy(root->data, text, MAX_LEN - 1);
    root->yes = NULL;
    root->no = NULL;
    root->is_leaf = is_leaf;
    return root;
}

void free_node(Node* node)
{
    free(node);
}

Node* create_tree()
{
    Node* root = create_node("Is that an animal?", 0);
    root->yes = create_node("Cat",1);
    root->no = create_node("Pushkin", 1);

    return root;
}
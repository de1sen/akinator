#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 256

typedef struct Node
{
    char data[MAX_LEN];
    struct Node* no;
    struct Node* yes;
    int is_leaf;
} Node;

Node* create_node(const char* text, int is_leaf);
void free_node(Node* node);
Node* create_tree();
void print_tree(Node* root, int depth);
void free_tree(Node* root);
int count_nodes(Node* node);
int count_leaves(Node* node);
int tree_depth(Node *node);
int ask_yes_no(const char *question);
void play(Node** root_ptr);

#endif
#ifndef JSON_IO_H
#define JSON_IO_H 

#include "tree.h"

void save_tree_json(Node* root, const char* filename);
Node* load_tree_json(const char* filename);

#endif
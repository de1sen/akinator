#ifndef FILE_IO_H
#define FILE_IO_H

#include "tree.h"

void save_tree(Node* root, const char* filename);
Node* load_tree(const char* filename);

#endif
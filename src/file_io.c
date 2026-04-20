#include "../include/file_io.h"

// Префиксный обход для сохранения
static void save_node(FILE* file, Node* node)
{
    if (node == NULL)
    {
        fprintf(file, "NULL\n");
        return;
    }
    
    fprintf(file, "%d\n", node->is_leaf);
    fprintf(file, "%s\n", node->data);
    save_node(file, node->no);
    save_node(file, node->yes);
}

void save_tree(Node* root, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Ошибка: не могу открыть файл %s для записи\n", filename);
        return;
    }
    
    save_node(file, root);
    fclose(file);
    printf("Дерево сохранено в %s\n", filename);
}

// Рекурсивная загрузка
static Node* load_node(FILE* file)
{
    int is_leaf;
    char buffer[MAX_LEN];
    
    if (fscanf(file, "%d\n", &is_leaf) != 1)
        return NULL;
    
    if (is_leaf == -1)  // Маркер NULL
        return NULL;
    
    fgets(buffer, MAX_LEN, file);
    buffer[strcspn(buffer, "\n")] = '\0';
    
    Node* node = create_node(buffer, is_leaf);
    node->no = load_node(file);
    node->yes = load_node(file);
    
    return node;
}

Node* load_tree(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Ошибка: не могу открыть файл %s для чтения\n", filename);
        return NULL;
    }
    
    Node* root = load_node(file);
    fclose(file);
    
    if (root)
        printf("Дерево загружено из %s\n", filename);
    
    return root;
}
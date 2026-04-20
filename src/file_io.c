#include "../include/file_io.h"

// Префиксный обход: записываем узел, потом левое поддерево, потом правое
static void save_node(FILE* file, Node* node)
{
    if (node == NULL)
    {
        // Специальный маркер для NULL
        fprintf(file, "NULL\n");
        return;
    }
    
    // Записываем данные узла
    fprintf(file, "NODE\n");
    fprintf(file, "%d\n", node->is_leaf);
    fprintf(file, "%s\n", node->data);
    
    // Рекурсивно сохраняем потомков
    save_node(file, node->no);
    save_node(file, node->yes);
}

void save_tree(Node* root, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Ошибка: не могу открыть %s для записи\n", filename);
        return;
    }
    
    fprintf(file, "=== AKINATOR TREE ===\n");
    save_node(file, root);
    fclose(file);
    printf("✅ Дерево сохранено (%d узлов)\n", count_nodes(root));
}

static Node* load_node(FILE* file)
{
    char line[MAX_LEN];
    
    // Читаем тип записи
    if (fgets(line, sizeof(line), file) == NULL)
        return NULL;
    
    // Убираем \n
    line[strcspn(line, "\n")] = '\0';
    
    // Если это NULL - возвращаем NULL
    if (strcmp(line, "NULL") == 0)
        return NULL;
    
    // Иначе это NODE - читаем данные
    if (strcmp(line, "NODE") != 0)
    {
        fprintf(stderr, "❌ Ошибка формата: ожидался NODE или NULL, получено '%s'\n", line);
        return NULL;
    }
    
    // Читаем is_leaf
    int is_leaf;
    if (fscanf(file, "%d\n", &is_leaf) != 1)
    {
        fprintf(stderr, "❌ Ошибка чтения is_leaf\n");
        return NULL;
    }
    
    // Читаем текст узла
    if (fgets(line, sizeof(line), file) == NULL)
    {
        fprintf(stderr, "❌ Ошибка чтения data\n");
        return NULL;
    }
    line[strcspn(line, "\n")] = '\0';
    
    // Создаём узел
    Node* node = create_node(line, is_leaf);
    if (node == NULL)
        return NULL;
    
    // Рекурсивно загружаем потомков
    node->no = load_node(file);
    node->yes = load_node(file);
    
    return node;
}

Node* load_tree(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "❌ Не могу открыть %s для чтения\n", filename);
        return NULL;
    }
    
    // Пропускаем заголовок
    char line[MAX_LEN];
    fgets(line, sizeof(line), file);
    
    Node* root = load_node(file);
    fclose(file);
    
    if (root)
        printf("✅ Дерево загружено (%d узлов)\n", count_nodes(root));
    
    return root;
}
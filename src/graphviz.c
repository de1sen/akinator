#include "../graphviz/graphviz.h"

static int node_counter = 0;

static void write_node_dot(FILE* file, Node* node, int parent_id)
{
    if (node == NULL)
        return;
    
    int current_id = node_counter++;
    
    // Форма узла
    const char* shape = node->is_leaf ? "box" : "ellipse";
    const char* color = node->is_leaf ? "lightgreen" : "lightblue";
    
    fprintf(file, "    node%d [label=\"%s\", shape=%s, style=filled, fillcolor=%s];\n",
            current_id, node->data, shape, color);
    
    if (parent_id != -1)
    {
        fprintf(file, "    node%d -> node%d;\n", parent_id, current_id);
    }
    
    write_node_dot(file, node->no, current_id);
    write_node_dot(file, node->yes, current_id);
}

void export_to_dot(Node* root, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Ошибка: не могу создать DOT файл\n");
        return;
    }
    
    fprintf(file, "digraph Akinator {\n");
    fprintf(file, "    node [fontname=\"Arial\"];\n");
    fprintf(file, "    edge [fontname=\"Arial\"];\n");
    
    node_counter = 0;
    write_node_dot(file, root, -1);
    
    fprintf(file, "}\n");
    fclose(file);
    
    printf("Граф сохранён в %s\n", filename);
    printf("Для просмотра выполните: dot -Tpng %s -o akinator.png\n", filename);
}
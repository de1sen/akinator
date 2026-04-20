#include "../include/tree.h"
#include "../include/file_io.h"
#include "../graphviz/graphviz.h"

void show_menu()
{
    printf("\n=== МЕНЮ АКИНАТОРА ===\n");
    printf("1. Играть\n");
    printf("2. Показать дерево\n");
    printf("3. Сохранить дерево\n");
    printf("4. Загрузить дерево\n");
    printf("5. Экспорт в GraphViz\n");
    printf("6. Статистика\n");
    printf("0. Выход\n");
    printf("Выберите действие: ");
}

int main(void)
{
    Node* tree = create_tree();
    int choice;
    
    printf("=== АКИНАТОР ===\n");
    
    do {
        show_menu();
        scanf("%d", &choice);
        getchar(); // Съедаем \n
        
        switch(choice)
        {
            case 1:
                play(&tree);
                break;
            case 2:
                print_tree(tree, 0);
                break;
            case 3:
                save_tree(tree, "data/tree_save.txt");
                break;
            case 4:
            {
                Node* loaded = load_tree("data/tree_save.txt");
                if (loaded)
                {
                    free_tree(tree);
                    tree = loaded;
                }
                break;
            }
            case 5:
                export_to_dot(tree, "data/akinator.dot");
                break;
            case 6:
                printf("\n=== СТАТИСТИКА ===\n");
                printf("Узлов: %d\n", count_nodes(tree));
                printf("Листьев: %d\n", count_leaves(tree));
                printf("Глубина: %d\n", tree_depth(tree));
                break;
            case 0:
                printf("До свидания!\n");
                break;
            default:
                printf("Неверный выбор\n");
        }
    } while(choice != 0);
    
    free_tree(tree);
    return 0;
}
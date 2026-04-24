#include "../include/tree.h"
#include "../include/file_io.h"
#include "../include/json_io.h"       // <-- ДОБАВИТЬ
#include "../graphviz/graphviz.h"

void show_menu()
{
    printf("\n=== МЕНЮ АКИНАТОРА ===\n");
    printf("1. Играть\n");
    printf("2. Показать дерево\n");
    printf("3. Сохранить дерево (TXT)\n");
    printf("4. Загрузить дерево (TXT)\n");
    printf("5. Экспорт в GraphViz\n");
    printf("6. Статистика\n");
    printf("7. Сбросить дерево (начать заново)\n");
    printf("8. Сохранить в JSON\n");         // <-- ДОБАВИТЬ
    printf("9. Загрузить из JSON\n");        // <-- ДОБАВИТЬ
    printf("0. Выход\n");
    printf("Выберите действие: ");
}

int main(void)
{
    Node* tree = NULL;
    printf("=== АКИНАТОР ===\n");

    // Пробуем загрузить JSON, если нет — TXT, если нет — создаём новое
    tree = load_tree_json("data/tree.json");
    if (tree == NULL)
    {
        tree = load_tree("data/tree_save.txt");
    }
    
    if (tree == NULL)
    {
        printf("Создаю новое дерево...\n");
        tree = create_tree();
        if (tree == NULL)
        {
            fprintf(stderr, "Ошибка создания дерева!\n");
            return 1;
        }
    }

    int choice;
    do {
        show_menu();
        scanf("%d", &choice);
        getchar();

        switch(choice)
        {
            case 1:
                play(&tree);
                // Автосохранение в оба формата
                save_tree(tree, "data/tree_save.txt");
                save_tree_json(tree, "data/tree.json");
                break;
            case 2:
                printf("\n=== ДЕРЕВО ===\n");
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
                }
                break;
            case 5:
                export_to_dot(tree, "data/akinator.dot");
                break;
            case 6:
                printf("\n=== СТАТИСТИКА ===\n");
                printf("Узлов: %d\n", count_nodes(tree));
                printf("Листьев: %d\n", count_leaves(tree));
                printf("Глубина: %d\n", tree_depth(tree));
                break;
            case 7:
                printf("Сбросить дерево? Все данные будут потеряны! (y/n): ");
                if (ask_yes_no(""))
                {
                    free_tree(tree);
                    tree = create_tree();
                    printf("Дерево сброшено\n");
                }
                break;
            case 8:                                          // <-- ДОБАВИТЬ
                save_tree_json(tree, "data/tree.json");      // <-- ДОБАВИТЬ
                break;                                       // <-- ДОБАВИТЬ
            case 9:                                          // <-- ДОБАВИТЬ
                {                                            // <-- ДОБАВИТЬ
                    Node* loaded = load_tree_json("data/tree.json");  // <-- ДОБАВИТЬ
                    if (loaded)                              // <-- ДОБАВИТЬ
                    {                                        // <-- ДОБАВИТЬ
                        free_tree(tree);                     // <-- ДОБАВИТЬ
                        tree = loaded;                       // <-- ДОБАВИТЬ
                    }                                        // <-- ДОБАВИТЬ
                }                                            // <-- ДОБАВИТЬ
                break;                                       // <-- ДОБАВИТЬ
            case 0:
                printf("Сохранить перед выходом? (y/n): ");
                if (ask_yes_no(""))
                {
                    save_tree(tree, "data/tree_save.txt");
                    save_tree_json(tree, "data/tree.json");
                }
                printf("До свидания!\n");
                break;
            default:
                printf("Неверный выбор\n");
        }
    } while(choice != 0);

    free_tree(tree);
    return 0;
}
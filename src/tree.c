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
    Node* root = create_node("Это животное?", 0);
    root->yes = create_node("Кошка",1);
    root->no = create_node("Пушкин", 1);

    return root;
}

void print_tree(Node* root, int depth)
{
    if (root == NULL)
        return;
    
    for (int i = 0; i < depth * 2; i++)
        putchar(' ');

    char * text = root->is_leaf == 1 ? "[*]" : "[?]";

    printf("%s %s\n", text, root->data);

    print_tree(root->no, depth+1);
    print_tree(root->yes, depth+1);
}

void free_tree(Node* root)
{
    if (root == NULL)
        return;
    
    free_tree(root->no);
    free_tree(root->yes);
    free(root);
}

int count_nodes(Node* node)
{
    if (node == NULL)
        return 0;
    
    return count_nodes(node->no) + count_nodes(node->yes) + 1;
}

int count_leaves(Node* node)
{
    if (node == NULL)
        return 0;
    if (node->is_leaf == 1)
        return 1;
    return count_leaves(node->no) + count_leaves(node->yes);  
}

int tree_depth(Node* node)
{
    if (node == NULL)
        return 0;

    int left = tree_depth(node->no);
    int right = tree_depth(node->yes);
    
    return (left > right ? left : right) + 1;
}

int ask_yes_no(const char *question)
{
    printf("%s\n", question);

    char answer[10];
    
    while (1)
    {
        fgets(answer, 10, stdin);

        answer[strcspn(answer, "\n")] = '\0';
        int len = strlen(answer);

        for (int i = 0; i < len; i++)
            answer[i] = tolower(answer[i]);

        if (strcmp("yes", answer) == 0 || strcmp("y", answer) == 0 || strcmp("да", answer) == 0)
            return 1;
        else if (strcmp("no", answer) == 0 || strcmp("n", answer) == 0 || strcmp("нет", answer) == 0)
            return 0;
        else 
            printf("Извините, я вас не понимаю, задам вопрос ещё раз.\n%s\n", question);
    }
}  

void play(Node** root_ptr)
{
    Node* current = *root_ptr;
    
    while (1)
    {
        if (current->is_leaf)
        {
            // Дошли до листа - делаем предположение
            printf("Я думаю, это %s!\n", current->data);
            
            if (ask_yes_no("Я угадал?"))
            {
                printf("Ура! Я выиграл!\n");
                printf("Хотите сыграть ещё? ");
                if (!ask_yes_no(""))
                    break;
                current = *root_ptr;  // Начинаем заново
            }
            else
            {
                // Акинатор проиграл - учимся
                printf("Я проиграл... Научите меня!\n");
                
                char new_answer[MAX_LEN];
                char new_question[MAX_LEN];
                
                printf("Кого вы загадали? ");
                fgets(new_answer, MAX_LEN, stdin);
                new_answer[strcspn(new_answer, "\n")] = '\0';
                
                printf("Какой вопрос отличает %s от %s?\n", new_answer, current->data);
                fgets(new_question, MAX_LEN, stdin);
                new_question[strcspn(new_question, "\n")] = '\0';
                
                printf("Какой ответ для %s? (да/нет): ", new_answer);
                int answer_for_new = ask_yes_no("");
                
                // Создаём новые узлы
                Node* old_answer_node = create_node(current->data, 1);
                Node* new_answer_node = create_node(new_answer, 1);
                Node* question_node = current;
                
                strncpy(question_node->data, new_question, MAX_LEN - 1);
                question_node->is_leaf = 0;
                
                if (answer_for_new)
                {
                    question_node->yes = new_answer_node;
                    question_node->no = old_answer_node;
                }
                else
                {
                    question_node->yes = old_answer_node;
                    question_node->no = new_answer_node;
                }
                
                printf("Спасибо! Теперь я знаю больше.\n");
                
                printf("Хотите сыграть ещё? ");
                if (!ask_yes_no(""))
                    break;
                current = *root_ptr;
            }
        }
        else
        {
            // Задаём вопрос
            if (ask_yes_no(current->data))
                current = current->yes;
            else
                current = current->no;
        }
    }
    
    printf("Спасибо за игру!\n");
}
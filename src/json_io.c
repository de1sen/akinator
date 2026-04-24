#include "../include/json_io.h"
#include "../lib/cJSON/cJSON.h"

// ========== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ==========

// Рекурсивно превращает узел дерева в JSON-объект
static cJSON* node_to_json(Node* node)
{
    if (node == NULL)
        return cJSON_CreateNull();  // null в JSON

    cJSON* json_node = cJSON_CreateObject();  // {}

    if (node->is_leaf)
    {
        // Лист: {"ответ": "Пушкин"}
        cJSON_AddStringToObject(json_node, "ответ", node->data);
    }
    else
    {
        // Вопрос: {"вопрос": "...", "нет": {...}, "да": {...}}
        cJSON_AddStringToObject(json_node, "вопрос", node->data);
        cJSON_AddItemToObject(json_node, "нет", node_to_json(node->no));
        cJSON_AddItemToObject(json_node, "да", node_to_json(node->yes));
    }

    return json_node;
}

// Рекурсивно превращает JSON-объект в узел дерева
static Node* json_to_node(cJSON* json_node)
{
    // null в JSON → NULL в дереве
    if (json_node == NULL || cJSON_IsNull(json_node))
        return NULL;

    // Проверяем, это лист или вопрос
    cJSON* answer = cJSON_GetObjectItem(json_node, "ответ");
    
    if (answer != NULL && cJSON_IsString(answer))
    {
        // Это лист — создаём узел-ответ
        return create_node(answer->valuestring, 1);
    }
    else
    {
        // Это вопрос — создаём узел-вопрос
        cJSON* question = cJSON_GetObjectItem(json_node, "вопрос");
        
        if (question == NULL || !cJSON_IsString(question))
        {
            fprintf(stderr, "❌ Ошибка: узел без 'вопрос' и без 'ответ'\n");
            return NULL;
        }

        Node* node = create_node(question->valuestring, 0);
        if (node == NULL)
            return NULL;

        // Рекурсивно создаём поддеревья
        node->no  = json_to_node(cJSON_GetObjectItem(json_node, "нет"));
        node->yes = json_to_node(cJSON_GetObjectItem(json_node, "да"));

        return node;
    }
}

// ========== ОСНОВНЫЕ ФУНКЦИИ ==========

void save_tree_json(Node* root, const char* filename)
{
    if (root == NULL)
    {
        fprintf(stderr, "❌ Ошибка: дерево пустое, нечего сохранять\n");
        return;
    }

    // Превращаем дерево в JSON
    cJSON* json_root = node_to_json(root);
    if (json_root == NULL)
    {
        fprintf(stderr, "❌ Ошибка создания JSON\n");
        return;
    }

    // Превращаем JSON в красивую строку
    char* json_string = cJSON_Print(json_root);
    if (json_string == NULL)
    {
        fprintf(stderr, "❌ Ошибка печати JSON\n");
        cJSON_Delete(json_root);
        return;
    }

    // Сохраняем в файл
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        fprintf(stderr, "❌ Не могу открыть %s для записи\n", filename);
        free(json_string);
        cJSON_Delete(json_root);
        return;
    }

    fprintf(file, "%s", json_string);
    fclose(file);

    printf("✅ Дерево сохранено в JSON: %s (%d узлов)\n", 
           filename, count_nodes(root));

    // Освобождаем память
    free(json_string);
    cJSON_Delete(json_root);
}

Node* load_tree_json(const char* filename)
{
    // Открываем файл
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "❌ Не могу открыть %s для чтения\n", filename);
        return NULL;
    }

    // Читаем весь файл в строку
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size <= 0)
    {
        fprintf(stderr, "❌ Файл %s пуст\n", filename);
        fclose(file);
        return NULL;
    }

    char* json_string = (char*)malloc(file_size + 1);
    if (json_string == NULL)
    {
        fprintf(stderr, "❌ Ошибка выделения памяти\n");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(json_string, 1, file_size, file);
    json_string[bytes_read] = '\0';
    fclose(file);

    // Парсим JSON
    cJSON* json_root = cJSON_Parse(json_string);

    // ❗ КОПИРУЕМ ошибку ДО освобождения json_string
    // Потому что cJSON_GetErrorPtr() указывает внутрь json_string!
    int has_error = 0;
    char error_buf[256] = {0};
    
    if (json_root == NULL)
    {
        has_error = 1;
        const char* err = cJSON_GetErrorPtr();
        if (err != NULL)
        {
            strncpy(error_buf, err, sizeof(error_buf) - 1);
        }
    }

    // ✅ Теперь МОЖНО освободить json_string
    free(json_string);
    json_string = NULL;

    // Проверяем ошибку парсинга
    if (has_error)
    {
        if (error_buf[0] != '\0')
        {
            fprintf(stderr, "❌ Ошибка парсинга JSON: %s\n", error_buf);
        }
        else
        {
            fprintf(stderr, "❌ Ошибка парсинга JSON\n");
        }
        return NULL;
    }

    // Превращаем JSON в дерево
    Node* tree = json_to_node(json_root);
    cJSON_Delete(json_root);

    if (tree == NULL)
    {
        fprintf(stderr, "❌ Ошибка: не удалось восстановить дерево из JSON\n");
        return NULL;
    }

    printf("✅ Дерево загружено из JSON: %s (%d узлов)\n",
           filename, count_nodes(tree));

    return tree;
}
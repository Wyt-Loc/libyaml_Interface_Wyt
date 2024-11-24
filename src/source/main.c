#define file1  "../../yamlfile/config_production.yaml"
#define file2  "../../yamlfile/config_setting.yaml"
#define file3  "../../yamlfile/config_software.yaml"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/debug/myDebug.h"
#include "../include/main.h"
#include "../include/readYaml.h"
#include "../include/writeYaml.h"

/*
// 读取功能
int main()
{
    Result result = getValueFromYaml(file3, "hal_config", "gnss", "baue");
    float port = 0;
    port = atof(result.values[0]);
    printf("%lf", port);
    free_result(&result);
    return 0;
}
*/

// 节点类型
typedef enum {
    YAML_SCALAR_NODE_TYPE,
    YAML_SEQUENCE_NODE_TYPE,
    YAML_MAPPING_NODE_TYPE
} NodeType;

// 链表节点结构
typedef struct YamlNode {
    char *key;                     // 键
    char *value;                   // 值（标量类型使用）
    NodeType type;                 // 节点类型
    struct YamlNode *children;     // 子节点（映射或序列的内容）
    struct YamlNode *next;         // 下一个链表节点
} YamlNode;

// 创建新的 YAML 节点
YamlNode *create_yaml_node(const char *key, const char *value, NodeType type) {
    YamlNode *node = (YamlNode *)malloc(sizeof(YamlNode));
    node->key = key ? strdup(key) : NULL;
    node->value = value ? strdup(value) : NULL;
    node->type = type;
    node->children = NULL;
    node->next = NULL;
    return node;
}

// 添加子节点
void append_yaml_node(YamlNode **head, YamlNode *node) {
    if (!node) {
        fprintf(stderr, "Error: Attempt to append NULL node.\n");
        return;
    }

    if (*head == NULL) {
        *head = node;
    } else {
        YamlNode *current = *head;
        while (current->next) {
            current = current->next;
        }
        current->next = node;
    }

    printf("Appended node: key = '%s', value = '%s', type = %d\n", 
           node->key ? node->key : "(null)", 
           node->value ? node->value : "(null)", 
           node->type);
}


// 遍历 YAML 节点
void traverse_yaml_document(yaml_document_t *document, yaml_node_t *node, YamlNode **head, const char *key) {
    if (!node) return;

    switch (node->type) {
        case YAML_SCALAR_NODE: {
            // 标量节点：存储键值对或序列项
            YamlNode *new_node = create_yaml_node(key, (char *)node->data.scalar.value, YAML_SCALAR_NODE_TYPE);
            append_yaml_node(head, new_node);
            break;
        }

        case YAML_SEQUENCE_NODE: {
            // 序列节点：存储键，同时递归解析子项
            YamlNode *sequence_node = create_yaml_node(key, NULL, YAML_SEQUENCE_NODE_TYPE);
            append_yaml_node(head, sequence_node);

            for (yaml_node_item_t *item = node->data.sequence.items.start; item < node->data.sequence.items.top; item++) {
                yaml_node_t *child = yaml_document_get_node(document, *item);
                traverse_yaml_document(document, child, &sequence_node->children, NULL);
            }
            break;
        }

        case YAML_MAPPING_NODE: {
            // 判断是否需要包一层
            YamlNode **current_head = head;

            if (key) { // 如果有 key，则创建包裹节点
                YamlNode *mapping_node = create_yaml_node(key, NULL, YAML_MAPPING_NODE_TYPE);
                append_yaml_node(head, mapping_node);
                current_head = &mapping_node->children;
            }

            for (yaml_node_pair_t *pair = node->data.mapping.pairs.start; pair < node->data.mapping.pairs.top; pair++) {
                yaml_node_t *key_node = yaml_document_get_node(document, pair->key);
                yaml_node_t *value_node = yaml_document_get_node(document, pair->value);

                char *key_str = key_node && key_node->type == YAML_SCALAR_NODE ? (char *)key_node->data.scalar.value : NULL;
                traverse_yaml_document(document, value_node, current_head, key_str);
            }
            break;
        }

        default:
            break;
    }
}

// 解析 YAML 文件
YamlNode *parse_yaml_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    yaml_parser_t parser;
    yaml_document_t document;

    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Failed to initialize parser\n");
        fclose(file);
        return NULL;
    }

    yaml_parser_set_input_file(&parser, file);

    YamlNode *root = NULL;
    if (yaml_parser_load(&parser, &document)) {
    yaml_node_t *root_node = yaml_document_get_root_node(&document);

    // 直接处理根节点
    if (root_node) {
        traverse_yaml_document(&document, root_node, &root, NULL);
    }

    yaml_document_delete(&document);
} else {
    fprintf(stderr, "Failed to parse YAML\n");
}

    yaml_parser_delete(&parser);
    fclose(file);

    return root;
}

// 打印链表
void print_yaml_node(YamlNode *node, int indent) {
    while (node) {
        for (int i = 0; i < indent; i++) printf("  ");
        if (node->type == YAML_SCALAR_NODE_TYPE) {
            printf("%s: %s\n", node->key ? node->key : "-", node->value);
        } else if (node->type == YAML_SEQUENCE_NODE_TYPE) {
            printf("%s:\n", node->key ? node->key : "-");
            print_yaml_node(node->children, indent + 1);
        } else if (node->type == YAML_MAPPING_NODE_TYPE) {
            printf("%s:\n", node->key);
            print_yaml_node(node->children, indent + 1);
        }
        node = node->next;
    }
}

// 释放链表内存
void free_yaml_node(YamlNode *node) {
    while (node) {
        YamlNode *next = node->next;
        if (node->key) free(node->key);
        if (node->value) free(node->value);
        free_yaml_node(node->children);
        free(node);
        node = next;
    }
}

void write_yaml_node(yaml_emitter_t *emitter, YamlNode *node, int is_root) {
    if (!node) return;

    yaml_event_t event;

    printf("nodetype = %d\r\n", node->type);

    while (node) {
        printf("Writing node: key = '%s', value = '%s', type = %d\n",
               node->key ? node->key : "(null)",
               node->value ? node->value : "(null)",
               node->type);

        if (node->type == YAML_SCALAR_NODE_TYPE) {
            // Write key if present
            if (node->key) {
                printf("1234\r\n");
                yaml_scalar_event_initialize(
                    &event, NULL, NULL, (unsigned char *)node->key, strlen(node->key),
                    1, 0, YAML_PLAIN_SCALAR_STYLE);
                if (!yaml_emitter_emit(emitter, &event)) {
                    fprintf(stderr, "Failed to emit key event 1\n");
                    return;
                }
            }

            // Write value if present
            if (node->value) {
                yaml_scalar_event_initialize(
                    &event, NULL, NULL, (unsigned char *)node->value, strlen(node->value),
                    1, 0, YAML_PLAIN_SCALAR_STYLE);
                if (!yaml_emitter_emit(emitter, &event)) {
                    fprintf(stderr, "Failed to emit value event\n");
                    return;
                }
            }
        } else if (node->type == YAML_MAPPING_NODE_TYPE) {
            // Write mapping start event with the key
            if (node->key) {
                yaml_scalar_event_initialize(
                    &event, NULL, NULL, (unsigned char *)node->key, strlen(node->key),
                    1, 0, YAML_PLAIN_SCALAR_STYLE);
                if (!yaml_emitter_emit(emitter, &event)) {
                    fprintf(stderr, "Failed to emit mapping key event\n");
                    return;
                }
            }

            yaml_mapping_start_event_initialize(&event, NULL, NULL, 1, YAML_BLOCK_MAPPING_STYLE);
            if (!yaml_emitter_emit(emitter, &event)) {
                fprintf(stderr, "Failed to begin mapping\n");
                return;
            }

            write_yaml_node(emitter, node->children, 0);  // Process children

            // Write mapping end event
            yaml_mapping_end_event_initialize(&event);
            if (!yaml_emitter_emit(emitter, &event)) {
                fprintf(stderr, "Failed to end mapping\n");
                return;
            }
        } else if (node->type == YAML_SEQUENCE_NODE_TYPE) {
            // Write sequence start event
            yaml_sequence_start_event_initialize(&event, NULL, NULL, 1, YAML_BLOCK_SEQUENCE_STYLE);
            if (!yaml_emitter_emit(emitter, &event)) {
                fprintf(stderr, "Failed to begin sequence\n");
                return;
            }

            write_yaml_node(emitter, node->children, 0);  // Process children

            // Write sequence end event
            yaml_sequence_end_event_initialize(&event);
            if (!yaml_emitter_emit(emitter, &event)) {
                fprintf(stderr, "Failed to end sequence\n");
                return;
            }
        }
        node = node->next;
    }
}


void write_yaml_file(const char *filename, YamlNode *root) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }

    yaml_emitter_t emitter;
    yaml_event_t event;

    if (!yaml_emitter_initialize(&emitter)) {
        fprintf(stderr, "Failed to initialize YAML emitter\n");
        fclose(file);
        return;
    }
    yaml_emitter_set_output_file(&emitter, file);

    // Start YAML stream
    yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING);
    if (!yaml_emitter_emit(&emitter, &event)) {
        fprintf(stderr, "Failed to emit YAML stream start event\n");
        yaml_emitter_delete(&emitter);
        fclose(file);
        return;
    }

    // Start YAML document
    yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 1);
    if (!yaml_emitter_emit(&emitter, &event)) {
        fprintf(stderr, "Failed to emit document start event\n");
        yaml_emitter_delete(&emitter);
        fclose(file);
        return;
    }

    // **检查根节点类型，必要时包装**
    if (root && root->type == YAML_SCALAR_NODE_TYPE) {
        printf("Wrapping root scalar node into a mapping\n");
        YamlNode *wrapped_root = create_yaml_node("root", NULL, YAML_MAPPING_NODE_TYPE);
        wrapped_root->children = root;
        root = wrapped_root; // 更新根节点为映射
    }

    // Write root node
    write_yaml_node(&emitter, root, 1);

    // End YAML document
    yaml_document_end_event_initialize(&event, 1);
    if (!yaml_emitter_emit(&emitter, &event)) {
        fprintf(stderr, "Failed to emit document end event\n");
        yaml_emitter_delete(&emitter);
        fclose(file);
        return;
    }

    // End YAML stream
    yaml_stream_end_event_initialize(&event);
    if (!yaml_emitter_emit(&emitter, &event)) {
        fprintf(stderr, "Failed to emit YAML stream end event\n");
        yaml_emitter_delete(&emitter);
        fclose(file);
        return;
    }

    // Clean up
    yaml_emitter_delete(&emitter);
    fclose(file);
}


int main()
{
    const char *input_file = "example.yaml";
    const char *output_file = "example1.yaml";

    // 解析 YAML 文件
    YamlNode *root = parse_yaml_file(input_file);
    if (root) {
        printf("解析的 YAML 结构为:\n");
        print_yaml_node(root, 0);

        // 写入 YAML 文件
        printf("开始写入\r\n");
        write_yaml_file(output_file, root);
        printf("链表内容已写入 %s\n", output_file);

        // 释放内存
        free_yaml_node(root);
    }

    return 0;
}

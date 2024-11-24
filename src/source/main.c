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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yaml.h>

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
    if (*head == NULL) {
        *head = node;
    } else {
        YamlNode *current = *head;
        while (current->next) {
            current = current->next;
        }
        current->next = node;
    }
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
            // 映射节点：存储键，同时递归解析子项
            YamlNode *mapping_node = create_yaml_node(key, NULL, YAML_MAPPING_NODE_TYPE);
            append_yaml_node(head, mapping_node);

            for (yaml_node_pair_t *pair = node->data.mapping.pairs.start; pair < node->data.mapping.pairs.top; pair++) {
                yaml_node_t *key_node = yaml_document_get_node(document, pair->key);
                yaml_node_t *value_node = yaml_document_get_node(document, pair->value);

                char *key_str = key_node && key_node->type == YAML_SCALAR_NODE ? (char *)key_node->data.scalar.value : NULL;
                traverse_yaml_document(document, value_node, &mapping_node->children, key_str);
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
        traverse_yaml_document(&document, root_node, &root, NULL);
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


// 写入单个 YAML 节点
void write_yaml_node(yaml_emitter_t *emitter, YamlNode *node) {
    yaml_event_t event;

    while (node) {
        if (node->type == YAML_SCALAR_NODE_TYPE) {
            if (node->key) {
                // 写入键
                yaml_scalar_event_initialize(&event, NULL, NULL,
                                             (yaml_char_t *)node->key, strlen(node->key),
                                             1, 1, YAML_PLAIN_SCALAR_STYLE);
                if (!yaml_emitter_emit(emitter, &event)) {
                    fprintf(stderr, "Failed to emit scalar key\n");
                    return;
                }
            }

            // 写入值
            yaml_scalar_event_initialize(&event, NULL, NULL,
                                         (yaml_char_t *)node->value, strlen(node->value),
                                         1, 1, YAML_PLAIN_SCALAR_STYLE);
            if (!yaml_emitter_emit(emitter, &event)) {
                fprintf(stderr, "Failed to emit scalar value\n");
                return;
            }

        } else if (node->type == YAML_SEQUENCE_NODE_TYPE) {
            if (node->key) {
                // 写入序列键
                yaml_scalar_event_initialize(&event, NULL, NULL,
                                             (yaml_char_t *)node->key, strlen(node->key),
                                             1, 1, YAML_PLAIN_SCALAR_STYLE);
                if (!yaml_emitter_emit(emitter, &event)) {
                    fprintf(stderr, "Failed to emit sequence key\n");
                    return;
                }
            }

            // 开始序列
            yaml_sequence_start_event_initialize(&event, NULL, NULL, 1, YAML_BLOCK_SEQUENCE_STYLE);
            if (!yaml_emitter_emit(emitter, &event)) {
                fprintf(stderr, "Failed to emit sequence start\n");
                return;
            }

            // 递归写入序列内容
            write_yaml_node(emitter, node->children);

            // 结束序列
            yaml_sequence_end_event_initialize(&event);
            if (!yaml_emitter_emit(emitter, &event)) {
                fprintf(stderr, "Failed to emit sequence end\n");
                return;
            }

        } else if (node->type == YAML_MAPPING_NODE_TYPE) {
            if (node->key) {
                // 写入映射键
                yaml_scalar_event_initialize(&event, NULL, NULL,
                                             (yaml_char_t *)node->key, strlen(node->key),
                                             1, 1, YAML_PLAIN_SCALAR_STYLE);
                if (!yaml_emitter_emit(emitter, &event)) {
                    fprintf(stderr, "Failed to emit mapping key\n");
                    return;
                }
            }

            // 开始映射
            yaml_mapping_start_event_initialize(&event, NULL, NULL, 1, YAML_BLOCK_MAPPING_STYLE);
            if (!yaml_emitter_emit(emitter, &event)) {
                fprintf(stderr, "Failed to emit mapping start\n");
                return;
            }

            // 递归写入映射内容
            write_yaml_node(emitter, node->children);

            // 结束映射
            yaml_mapping_end_event_initialize(&event);
            if (!yaml_emitter_emit(emitter, &event)) {
                fprintf(stderr, "Failed to emit mapping end\n");
                return;
            }
        }

        node = node->next;
    }
}

// 写入链表内容到 YAML 文件
void write_yaml_to_file(const char *filename, YamlNode *root) {
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

    // 开始 YAML 文档
    yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING);
    if (!yaml_emitter_emit(&emitter, &event)) {
        fprintf(stderr, "Failed to emit stream start\n");
        yaml_emitter_delete(&emitter);
        fclose(file);
        return;
    }

    yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 1);
    if (!yaml_emitter_emit(&emitter, &event)) {
        fprintf(stderr, "Failed to emit document start\n");
        yaml_emitter_delete(&emitter);
        fclose(file);
        return;
    }

    // 写入链表内容
    write_yaml_node(&emitter, root);

    // 结束 YAML 文档
    yaml_document_end_event_initialize(&event, 1);
    if (!yaml_emitter_emit(&emitter, &event)) {
        fprintf(stderr, "Failed to emit document end\n");
        yaml_emitter_delete(&emitter);
        fclose(file);
        return;
    }

    yaml_stream_end_event_initialize(&event);
    if (!yaml_emitter_emit(&emitter, &event)) {
        fprintf(stderr, "Failed to emit stream end\n");
        yaml_emitter_delete(&emitter);
        fclose(file);
        return;
    }

    yaml_emitter_delete(&emitter);
    fclose(file);
}

// 在链表中查找指定键，并在其后插入新键值对
int insert_after_key(YamlNode *head, const char *target_key, const char *new_key, const char *new_value, int new_type) {
    YamlNode *current = head;

    // 遍历链表查找目标键
    while (current) {
        if (current->key && strcmp(current->key, target_key) == 0) {
            // 找到目标键
            YamlNode *new_node = create_yaml_node(new_key, new_value, new_type);
            new_node->next = current->next; // 将新节点连接到链表
            current->next = new_node;      // 将当前节点指向新节点
            return 1; // 插入成功
        }
        current = current->next;
    }

    return 0; // 未找到目标键
}


// 递归修改链表中指定根节点下的目标键的值
// 递归修改链表中指定根节点下的目标键的值
int modify_key_value(YamlNode *head, const char *target_key, const char *new_value, const char *target_root) {
    YamlNode *current = head;

    // 如果当前节点为空，说明已经遍历完了
    if (current == NULL) {
        return 0; // 未找到目标键
    }

    printf("%s, %s\r\n", current->key, target_root);

    // 如果当前节点是目标根节点，开始处理
    if (current->key && strcmp(current->key, target_root) == 0) {
        // 遍历当前节点下的链表
        while (current) {
            printf("kkkey == %s\r\n", current->key);

            // 如果找到目标键，则修改其值
            if (current->key && strcmp(current->key, target_key) == 0) {
                // 找到目标键，修改值
                if (current->value) {
                    free(current->value); // 释放旧值
                }
                current->value = strdup(new_value); // 分配新值
                return 1; // 修改成功
            }

            // 递归修改当前节点的子节点
            if (current->children) {
                if (modify_key_value(current->children, target_key, new_value, target_root)) {
                    return 1; // 成功修改子节点
                }
            }
        }
                    // 移动到下一个节点
            current = current->next;
    }


    return 0; // 未找到目标键
}


int main() {
    const char *input_file = "example.yaml";
    const char *output_file = "example.yaml";

    // 解析 YAML 文件
    YamlNode *root = parse_yaml_file(input_file);
    if (root) {
        printf("Parsed YAML content:\n");
        print_yaml_node(root, 0);

        // 修改键值
        const char *target_key = "name";
        const char *target_root = "protocol1";
        const char *new_value = "Jane Doe";

        if (modify_key_value(root, target_key, new_value, target_root)) {
            printf("\nKey '%s' updated to new value: '%s'\n", target_key, new_value);
        } else {
            printf("\nKey '%s' not found.\n", target_key);
        }

        // 打印修改后的内容
        printf("\nModified YAML content:\n");
        print_yaml_node(root, 0);

        // 写入修改后的内容到新文件
        write_yaml_to_file(output_file, root);

        printf("\nModified YAML content has been written to %s\n", output_file);

        // 释放内存
        free_yaml_node(root);
    } else {
        printf("Failed to parse YAML file.\n");
    }

    return 0;
}

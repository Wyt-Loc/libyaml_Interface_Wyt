#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yaml.h>
#include "../include/readYaml.h"


// 递归函数，用于遍历并查找指定的键的值
void traverse_yaml_node(yaml_document_t *document, yaml_node_t *node, const char *root, const char *sub_key, const char *key, Result *result, int *found_root, int *found_sub_key, int *found_key, int *found) {
    if (!node || *found) return;  // 如果已经找到目标，直接返回

    switch (node->type) {
        case YAML_SCALAR_NODE:
            if (*found_key) {
                result->values[result->count] = (float*)(strdup((char *)node->data.scalar.value));
                result->count++;
                *found_key = 0;  // 重置标志以避免重复输出
                *found = 1;  // 设置标志表示已找到
            } else if ((*found_root || root == NULL) && (*found_sub_key || sub_key == NULL)) {
                if (strcmp((char *)node->data.scalar.value, key) == 0) {
                    *found_key = 1;
                }
            } else if (root && strcmp((char *)node->data.scalar.value, root) == 0) {
                *found_root = 1;
            } else if (*found_root && sub_key && strcmp((char *)node->data.scalar.value, sub_key) == 0) {
                *found_sub_key = 1;
            }
            break;

        case YAML_SEQUENCE_NODE:
            if (*found_sub_key || sub_key == NULL) {
                for (yaml_node_item_t *item = node->data.sequence.items.start; item < node->data.sequence.items.top; item++) {
                    yaml_node_t *child = yaml_document_get_node(document, *item);
                    if (child) {
                        if (child->type == YAML_SCALAR_NODE) {
                            traverse_yaml_node(document, child, root, sub_key, key, result, found_root, found_sub_key, found_key, found);
                        } else if (child->type == YAML_MAPPING_NODE) {
                            traverse_yaml_node(document, child, root, sub_key, key, result, found_root, found_sub_key, found_key, found);
                        }
                    }
                }
            }
            break;

        case YAML_MAPPING_NODE:
            for (yaml_node_pair_t *pair = node->data.mapping.pairs.start; pair < node->data.mapping.pairs.top; pair++) {
                yaml_node_t *key_node = yaml_document_get_node(document, pair->key);
                yaml_node_t *value_node = yaml_document_get_node(document, pair->value);

                if (key_node && key_node->type == YAML_SCALAR_NODE) {
                    traverse_yaml_node(document, key_node, root, sub_key, key, result, found_root, found_sub_key, found_key, found);
                }
                if (value_node) {
                    traverse_yaml_node(document, value_node, root, sub_key, key, result, found_root, found_sub_key, found_key, found);
                }
            }
            break;

        default:
            break;
    }
}

Result getValueFromYaml(const char *filename, const char *root, const char *sub_key, const char *key) {
    Result result = {NULL, 0};
    result.values = malloc(100 * sizeof(char*));

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return result;
    }

    yaml_parser_t parser;
    yaml_document_t document;

    if (!yaml_parser_initialize(&parser)) {
        fputs("Failed to initialize parser!\n", stderr);
        fclose(file);
        return result;
    }

    yaml_parser_set_input_file(&parser, file);

    int found_root = (root == NULL);
    int found_sub_key = (sub_key == NULL);
    int found_key = 0;
    int found = 0; // 新增标志，用于表示是否已经找到目标

    while (yaml_parser_load(&parser, &document)) {
        yaml_node_t *root_node = yaml_document_get_root_node(&document);
        if (!root_node) {
            fputs("Failed to get root node!\n", stderr);
            yaml_document_delete(&document);
            break;
        }

        traverse_yaml_node(&document, root_node, root, sub_key, key, &result, &found_root, &found_sub_key, &found_key, &found);

        yaml_document_delete(&document);
        if (found) break; // 一旦找到目标键就退出
    }

    yaml_parser_delete(&parser);
    fclose(file);

    return result;
}

// 释放结果占用的内存
void free_result(Result *result) {
    for (size_t i = 0; i < result->count; i++) {
        free(result->values[i]);
    }
    free(result->values);
}

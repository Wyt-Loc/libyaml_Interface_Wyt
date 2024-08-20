/*
 * @Author: Wyt 1697556601@qq.com
 * @Date: 2024-08-10 14:57:28
 * @LastEditors: Wyt 1697556601@qq.com
 * @LastEditTime: 2024-08-20 23:22:21
 * @FilePath: /libyaml_Interface_Wyt/src/source/getAllData.c
 * @Description:
 *
 * Copyright (c) 2024 by Wyt, All Rights Reserved.
 */
#include <yaml.h>
#include <stdio.h>


yaml_char_t* getKey[100];
yaml_char_t* GetValue[1024];

char* keyFlag = "key";

// 递归函数，用于遍历并打印所有节点的类型和内容
void traverse_yaml_node(yaml_document_t *document, yaml_node_t *node, int depth) {
    if (!node) return;

    // 打印缩进和节点类型
    for (int i = 0; i < depth; i++) printf("  ");

    switch (node->type) {
        case YAML_SCALAR_NODE:
        if(strcmp(keyFlag, "key") == 0)
        {
            printf("Key scalar: %s\n", node->data.scalar.value);
        }
        if (strcmp(keyFlag, "value") == 0)
        {
            printf("value scalar: %s\n", node->data.scalar.value);
        }
        break;

        case YAML_SEQUENCE_NODE:
            printf("Sequence:\n");
            for (yaml_node_item_t *item = node->data.sequence.items.start;
                 item < node->data.sequence.items.top; item++) {
                yaml_node_t *child = yaml_document_get_node(document, *item);
                traverse_yaml_node(document, child, depth + 1);
            }
            break;

        case YAML_MAPPING_NODE:
            printf("Mapping:\n");
            for (yaml_node_pair_t *pair = node->data.mapping.pairs.start;
                 pair < node->data.mapping.pairs.top; pair++) {
                yaml_node_t *key = yaml_document_get_node(document, pair->key);
                yaml_node_t *value = yaml_document_get_node(document, pair->value);

                for (int i = 0; i < depth + 1; i++) printf("  ");
                keyFlag = "key";
                traverse_yaml_node(document, key, 0);

                for (int i = 0; i < depth + 1; i++) printf("  ");
                keyFlag = "value";
                traverse_yaml_node(document, value, depth + 2);

            }
            break;

        default:
            printf("Unknown node type\n");
            break;
    }
}

int getAllData() {
    FILE *file = fopen("../../yamlfile/input.yaml", "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    yaml_parser_t parser;
    yaml_document_t document;

    if (!yaml_parser_initialize(&parser)) {
        fputs("Failed to initialize parser!\n", stderr);
        return 1;
    }

    yaml_parser_set_input_file(&parser, file);

    yaml_document_get_node(&document, 2);

    while (yaml_parser_load(&parser, &document)) {
        // 获取文档的根节点
        yaml_node_t *root = yaml_document_get_root_node(&document);
        if (!root) {
            fputs("Failed to get root node!\n", stderr);
            yaml_document_delete(&document);
            break;;
        }

        // 遍历并处理整个文档的节点
        traverse_yaml_node(&document, root, 0);

        // 清理当前文档
        yaml_document_delete(&document);
    }

    // 检查是否发生了解析错误
    if (parser.error != YAML_NO_ERROR) {
        fprintf(stderr, "Parser error %d occurred!\n", parser.error);
    }

    // 清理
    yaml_parser_delete(&parser);
    fclose(file);

    return 0;
}

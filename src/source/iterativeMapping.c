/*
 * @Author: Wyt 1697556601@qq.com
 * @Date: 2024-08-10 13:45:27
 * @LastEditors: Wyt 1697556601@qq.com
 * @LastEditTime: 2024-08-10 14:54:21
 * @FilePath: /libyaml_Interface_Wyt/src/source/iterativeMapping.c
 * @Description: 
 * 
 * Copyright (c) 2024 by Wyt, All Rights Reserved. 
 */
#include <yaml.h>
#include <stdio.h>

int iterativeMapping(void) {
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


 while (yaml_parser_load(&parser, &document)) {

    // Get the root node of the document
    yaml_node_t *root = yaml_document_get_root_node(&document);
    if (!root || root->type != YAML_MAPPING_NODE) {
        fputs("Root node is not a mapping!\n", stderr);
        yaml_document_delete(&document);
        yaml_parser_delete(&parser);
        fclose(file);
        return 1;
    }

    // Iterate over all key-value pairs in the mapping
    for (yaml_node_pair_t *pair = root->data.mapping.pairs.start;
         pair < root->data.mapping.pairs.top; pair++) {

        yaml_node_t *keyNode = yaml_document_get_node(&document, pair->key);
        yaml_node_t *valueNode = yaml_document_get_node(&document, pair->value);

        if (keyNode->type == YAML_SCALAR_NODE) {
            printf("Key: %s\n", keyNode->data.scalar.value);
        }

        if (valueNode->type == YAML_SCALAR_NODE) {
            printf("Value: %s\n", valueNode->data.scalar.value);
        } else if (valueNode->type == YAML_SEQUENCE_NODE) {
            printf("Value is a sequence.\n");
            // You can further iterate over the sequence if needed
        } else if (valueNode->type == YAML_MAPPING_NODE) {
            printf("Value is a mapping.\n");
            // You can recursively process this mapping if needed
        }
    }

    // Clean up
    yaml_document_delete(&document);
 }
    yaml_parser_delete(&parser);
    fclose(file);

    return 0;
}

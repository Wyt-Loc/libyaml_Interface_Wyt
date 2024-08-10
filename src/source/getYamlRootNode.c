/*
 * @Author: Wyt 1697556601@qq.com
 * @Date: 2024-08-10 13:35:54
 * @LastEditors: Wyt 1697556601@qq.com
 * @LastEditTime: 2024-08-10 14:49:43
 * @FilePath: /libyaml_Interface_Wyt/src/source/getYamlRootNode.c
 * @Description: 
 * 
 * Copyright (c) 2024 by Wyt, All Rights Reserved. 
 */

#include "../include/getYamlRootNode.h"

int getYamlFileRootNode(const char* fileName, const char* openMode) {
    

    FILE *file = fopen(fileName, openMode);
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

    int doc_count = 0;
    while (yaml_parser_load(&parser, &document)) {
        // Get the root node of the current document
        yaml_node_t *root = yaml_document_get_root_node(&document);
        if (!root) {
            fprintf(stderr, "Failed to get root node in document %d!\n", doc_count);
            yaml_document_delete(&document);
            break;
        }
         doc_count++;
        // Print the type of the root node
        switch (root->type) {
            case YAML_MAPPING_NODE:
                printf("Root node in document %d is a mapping.\n", doc_count);
                break;
            case YAML_SEQUENCE_NODE:
                printf("Root node in document %d is a sequence.\n", doc_count);
                break;
            case YAML_SCALAR_NODE:
                printf("Root node in document %d is a scalar: %s\n", doc_count, root->data.scalar.value);
                break;
            default:
                printf("Unknown node type in document %d.\n", doc_count);
                break;
        }
        // Clean up the document before loading the next one
        yaml_document_delete(&document);
    }

    // Check if an error occurred during parsing
    if (parser.error != YAML_NO_ERROR) {
        fprintf(stderr, "Parser error %d occurred!\n", parser.error);
    }
    
    // Clean up
    yaml_parser_delete(&parser);
    fclose(file);

    return 0;
}

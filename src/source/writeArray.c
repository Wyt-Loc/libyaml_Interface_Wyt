/*
 * @Author: Wyt 1697556601@qq.com
 * @Date: 2024-08-08 00:32:23
 * @LastEditors: Wyt 1697556601@qq.com
 * @LastEditTime: 2024-08-20 23:21:36
 * @FilePath: /libyaml_Interface_Wyt/src/source/writeArray.c
 * @Description: 
 * 
 * Copyright (c) 2024 by Wyt, All Rights Reserved. 
 */


#include <stdio.h>
#include <yaml.h>
#include <string.h>
#include "../include/writeArray.h"


// Array of key-value pairs
KeyValueStore* createKeyValueStore(const char *keyName, int numValues) {  
    KeyValueStore *store = (KeyValueStore*)malloc(sizeof(KeyValueStore));  
    if (!store) return NULL;
    store->keyName = strdup(keyName); // 复制键名
    if (!store->keyName) {
        free(store);
        return NULL;
    }
    store->values = (char**)malloc(numValues * sizeof(char*));
    if (!store->values) {
        free(store->keyName);
        free(store);
        return NULL;
    }

    store->numValues = numValues;

    // 初始化values数组中的每个元素为NULL（如果需要的话）
    for (int i = 0; i < numValues; i++) {
        store->values[i] = NULL;
    }

    return store;
}

// 添加键值
void addValue(KeyValueStore *store, int index, const char *value) {
    if (index >= store->numValues) {
        fprintf(stderr, "Index out of bounds\n");
        return;
    }

    store->values[index] = strdup(value); // 复制键值
    if (!store->values[index]) {
        fprintf(stderr, "Memory allocation failed\n");
    }
}

// 清理资源
void freeKeyValueStore(KeyValueStore *store) {
    if (!store) return;

    free(store->keyName);
    for (int i = 0; i < store->numValues; i++) {
        free(store->values[i]);
    }
    free(store->values);
    free(store);
}


int writeYamlArray(const char* fileName, const char* openMode, KeyValueStore *store){

    FILE *file = fopen(fileName, openMode);
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    yaml_emitter_t emitter;
    yaml_event_t event;

    if (!yaml_emitter_initialize(&emitter)) {
        fputs("Failed to initialize emitter!\n", stderr);
        return 1;
    }

    yaml_emitter_set_output_file(&emitter, file);

    // 初始化发射器
    if (!yaml_emitter_initialize(&emitter)) {
        fputs("Failed to initialize emitter!\n", stderr);
        fclose(file);
        return 1;
    }

    // 设置输出到文件
    yaml_emitter_set_output_file(&emitter, file);

    // 创建并发射流开始事件
    yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING);
    yaml_emitter_emit(&emitter, &event);

    // 创建并发射文档开始事件
    yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 1);
    yaml_emitter_emit(&emitter, &event);

    // 创建并发射映射开始事件
    yaml_mapping_start_event_initialize(&event, NULL, NULL, 0, YAML_BLOCK_MAPPING_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // 创建并发射标量事件 (键)
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)store->keyName,
                                 -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // Start the sequence (array)
    if (!yaml_sequence_start_event_initialize(&event, NULL, NULL, 1, YAML_BLOCK_SEQUENCE_STYLE) ||
        !yaml_emitter_emit(&emitter, &event)) {
        fputs("Failed to start sequence!\n", stderr);
        return 1;
    }

    // Add elements to the array
    for (int i = 0; i < store->numValues; i++) {
        if (!yaml_scalar_event_initialize(&event, NULL, NULL,
                                          (yaml_char_t *)store->values[i],
                                          strlen((const char*)store->values[i]),
                                          1, 1, YAML_PLAIN_SCALAR_STYLE) ||
            !yaml_emitter_emit(&emitter, &event)) {
            fprintf(stderr, "Failed to write array element %d!\n", i);
            return 1;
        }
    }

    // End the sequence
    yaml_sequence_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);

    // 创建并发射映射结束事件
    yaml_mapping_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);

    // 创建并发射文档结束事件
    yaml_document_end_event_initialize(&event, 1);
    yaml_emitter_emit(&emitter, &event);

    // 创建并发射流结束事件
    yaml_stream_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);

    // 清理发射器
    yaml_emitter_delete(&emitter);

    // 关闭文件
    fclose(file);
    return 0;

}






int writeYamlDepthTwo(void)
{

    FILE *fh = fopen("../../yamlfile/input.yaml", "a");
    if (!fh) {
        perror("Failed to open file");
        return 1;
    }

    yaml_emitter_t emitter;
    yaml_event_t event;

    // Initialize the emitter
    if (!yaml_emitter_initialize(&emitter)) {
        fputs("Failed to initialize emitter!\n", stderr);
        return 1;
    }

    // Set the output file
    yaml_emitter_set_output_file(&emitter, fh);

    // Start the YAML stream
    yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING);
    yaml_emitter_emit(&emitter, &event);

    // Start the document
    yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 1);
    yaml_emitter_emit(&emitter, &event);

    // Start the root mapping
    yaml_mapping_start_event_initialize(&event, NULL, NULL, 1, YAML_BLOCK_MAPPING_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // Write "NetWork" key
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"NetWork", strlen("NetWork"), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // Start the NetWork mapping
    yaml_mapping_start_event_initialize(&event, NULL, NULL, 1, YAML_BLOCK_MAPPING_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // Write "TCP" key
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"TCP", strlen("TCP"), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // Start the TCP sequence
    yaml_sequence_start_event_initialize(&event, NULL, NULL, 1, YAML_BLOCK_SEQUENCE_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // Write TCP port
    yaml_mapping_start_event_initialize(&event, NULL, NULL, 1, YAML_BLOCK_MAPPING_STYLE);
    yaml_emitter_emit(&emitter, &event);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"port", strlen("port"), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"3306", strlen("3306"), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    yaml_mapping_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);

    yaml_sequence_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);

    // Write "UDP" key
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"UDP", strlen("UDP"), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // Start the UDP sequence
    yaml_sequence_start_event_initialize(&event, NULL, NULL, 1, YAML_BLOCK_SEQUENCE_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // Write UDP port
    yaml_mapping_start_event_initialize(&event, NULL, NULL, 1, YAML_BLOCK_MAPPING_STYLE);
    yaml_emitter_emit(&emitter, &event);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"port", strlen("port"), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"5432", strlen("5432"), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    yaml_mapping_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);

    yaml_sequence_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);

    // End the NetWork mapping
    yaml_mapping_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);

    // End the root mapping
    yaml_mapping_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);

    // End the document
    yaml_document_end_event_initialize(&event, 1);
    yaml_emitter_emit(&emitter, &event);

    // End the YAML stream
    yaml_stream_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);

    // Cleanup
    yaml_emitter_delete(&emitter);
    fclose(fh);

    return 0;

}



#define MAX_ARRAY_SIZE 100
#define MAX_KEY_LEN 100
#define MAX_VALUE_LEN 100

typedef struct {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
} YamlPair;

typedef struct {
    YamlPair pairs[MAX_ARRAY_SIZE];
    int count;
} YamlArray;

void parse_yaml_node(yaml_parser_t *parser, YamlArray *array, char *current_key, int is_in_sequence);

void parse_yaml_file(const char *filename, YamlArray *array) {
    FILE *fh = fopen(filename, "r");
    if (!fh) {
        perror("Failed to open file");
        return;
    }

    yaml_parser_t parser;
    yaml_event_t event;

    yaml_parser_initialize(&parser);
    yaml_parser_set_input_file(&parser, fh);

    char current_key[MAX_KEY_LEN] = "";

    while (1) {
        if (!yaml_parser_parse(&parser, &event)) {
            printf("Parser error %d\n", parser.error);
            break;
        }

        if (event.type == YAML_STREAM_END_EVENT) {
            yaml_event_delete(&event);
            break;
        }

        if (event.type == YAML_SCALAR_EVENT) {
            if (strlen(current_key) == 0) {
                // This is a key
                strncpy(current_key, (char *)event.data.scalar.value, MAX_KEY_LEN);
            } else {
                // This is a value
                if (array->count < MAX_ARRAY_SIZE) {
                    strncpy(array->pairs[array->count].key, current_key, MAX_KEY_LEN);
                    strncpy(array->pairs[array->count].value, (char *)event.data.scalar.value, MAX_VALUE_LEN);
                    array->count++;
                }
                current_key[0] = '\0';  // Reset current_key for next pair
            }
        } else if (event.type == YAML_MAPPING_START_EVENT) {
            if (strlen(current_key) > 0) {
                strcat(current_key, ".");  // Nested key, add a dot notation
            }
            parse_yaml_node(&parser, array, current_key, 0);
            // Remove the dot if it's there
            if (strlen(current_key) > 0 && current_key[strlen(current_key) - 1] == '.') {
                current_key[strlen(current_key) - 1] = '\0';  // Remove the dot notation after returning
            }
        } else if (event.type == YAML_SEQUENCE_START_EVENT) {
            parse_yaml_node(&parser, array, current_key, 1);
        } else if (event.type == YAML_MAPPING_END_EVENT || event.type == YAML_SEQUENCE_END_EVENT) {
            break;
        }

        yaml_event_delete(&event);
    }

    yaml_parser_delete(&parser);
    fclose(fh);
}

void parse_yaml_node(yaml_parser_t *parser, YamlArray *array, char *current_key, int is_in_sequence) {
    yaml_event_t event;
    int item_index = 0;
    char nested_key[MAX_KEY_LEN];

    while (1) {
        if (!yaml_parser_parse(parser, &event)) {
            printf("Parser error %d\n", parser->error);
            return;
        }

        if (event.type == YAML_SCALAR_EVENT) {
            if (is_in_sequence) {
                snprintf(nested_key, MAX_KEY_LEN, "%s[%d]", current_key, item_index++);
                if (array->count < MAX_ARRAY_SIZE) {
                    strncpy(array->pairs[array->count].key, nested_key, MAX_KEY_LEN);
                    strncpy(array->pairs[array->count].value, (char *)event.data.scalar.value, MAX_VALUE_LEN);
                    array->count++;
                }
            } else {
                if (strlen(current_key) > 0 && current_key[strlen(current_key) - 1] == '.') {
                    current_key[strlen(current_key) - 1] = '\0';  // Remove the dot notation
                }
                if (array->count < MAX_ARRAY_SIZE) {
                    strncpy(array->pairs[array->count].key, current_key, MAX_KEY_LEN);
                    strncpy(array->pairs[array->count].value, (char *)event.data.scalar.value, MAX_VALUE_LEN);
                    array->count++;
                }
            }
        } else if (event.type == YAML_MAPPING_START_EVENT) {
            strcat(current_key, ".");  // Add dot notation for nested keys
            parse_yaml_node(parser, array, current_key, 0);
            // Remove the dot notation
            if (strlen(current_key) > 0 && current_key[strlen(current_key) - 1] == '.') {
                current_key[strlen(current_key) - 1] = '\0';
            }
        } else if (event.type == YAML_SEQUENCE_START_EVENT) {
            parse_yaml_node(parser, array, current_key, 1);
        } else if (event.type == YAML_MAPPING_END_EVENT || event.type == YAML_SEQUENCE_END_EVENT) {
            break;
        }

        yaml_event_delete(&event);
    }
}


int readDataaa(void) {
    YamlArray array;
    array.count = 0;

    parse_yaml_file("../../yamlfile/input.yaml", &array);

    // Output the parsed content
    for (int i = 0; i < array.count; i++) {
        printf("Key: %s, Value: %s\n", array.pairs[i].key, array.pairs[i].value);
    }

    return 0;
}

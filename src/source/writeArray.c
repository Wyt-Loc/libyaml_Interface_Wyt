/*
 * @Author: Wyt 1697556601@qq.com
 * @Date: 2024-08-08 00:32:23
 * @LastEditors: Wyt 1697556601@qq.com
 * @LastEditTime: 2024-08-10 21:01:38
 * @FilePath: /libyaml_Interface_Wyt/src/source/writeArray.c
 * @Description: 
 * 
 * Copyright (c) 2024 by Wyt, All Rights Reserved. 
 */


#include <stdio.h>
#include <yaml.h>
#include <string.h>
#include "../include/writeArray.h"


// 初始化KeyValueStore
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
    yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 0);
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
    yaml_document_end_event_initialize(&event, 0);
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

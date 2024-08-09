/*
 * @Author: Wyt 1697556601@qq.com
 * @Date: 2024-08-08 00:32:23
 * @LastEditors: Wyt 1697556601@qq.com
 * @LastEditTime: 2024-08-10 00:50:28
 * @FilePath: /libyaml_Interface_Wyt/src/source/writeArray.c
 * @Description: 
 * 
 * Copyright (c) 2024 by Wyt, All Rights Reserved. 
 */


#include <stdio.h>
#include <yaml.h>
#include "../include/writeArray.h"


int writeYamlArray(void){

    yaml_emitter_t emitter;
    yaml_event_t event;
    FILE *file;

    // 打开文件进行写入
    file = fopen("output.yaml", "w");
    if (file == NULL) {
        fputs("Failed to open file for writing!\n", stderr);
        return 1;
    }

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

    // 创建并发射序列开始事件
    yaml_sequence_start_event_initialize(&event, NULL, NULL, 0, YAML_BLOCK_SEQUENCE_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // 创建并发射数组中的每个元素
    const char *array[] = {"item1", "item2", "item3"};
    for (int i = 0; i < 3; i++) {
        yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)array[i],
                                     -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
        yaml_emitter_emit(&emitter, &event);
    }

    // 创建并发射序列结束事件
    yaml_sequence_end_event_initialize(&event);
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
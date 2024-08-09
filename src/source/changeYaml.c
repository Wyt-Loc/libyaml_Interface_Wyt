/*
 * @Author: Wyt 1697556601@qq.com
 * @Date: 2024-08-08 00:32:23
 * @LastEditors: Wyt 1697556601@qq.com
 * @LastEditTime: 2024-08-10 01:15:02
 * @FilePath: /libyaml_Interface_Wyt/src/source/changeYaml.c
 * @Description: 
 * 
 * Copyright (c) 2024 by Wyt, All Rights Reserved. 
 */


#include <stdio.h>

#include "../include/changeYaml.h"


void write_yaml_with_new_data(FILE *file) {
    yaml_emitter_t emitter;
    yaml_event_t event;

    // 初始化发射器
    if (!yaml_emitter_initialize(&emitter)) {
        fputs("Failed to initialize emitter!\n", stderr);
        fclose(file);
        return;
    }

    // 设置输出到文件
    yaml_emitter_set_output_file(&emitter, file);

    // 创建并发射流开始事件
    yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING);
    yaml_emitter_emit(&emitter, &event);
    yaml_event_delete(&event);  // 删除事件

    // 创建并发射文档开始事件
    yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 0);
    yaml_emitter_emit(&emitter, &event);
    yaml_event_delete(&event);  // 删除事件

    // 创建并发射映射开始事件 (顶级映射)
    yaml_mapping_start_event_initialize(&event, NULL, NULL, 0, YAML_BLOCK_MAPPING_STYLE);
    yaml_emitter_emit(&emitter, &event);
    yaml_event_delete(&event);  // 删除事件

    // 创建并发射标量事件 (键)
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"A",
                                 -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);
    yaml_event_delete(&event);  // 删除事件

    // 创建并发射序列开始事件 (值)
    yaml_sequence_start_event_initialize(&event, NULL, NULL, 0, YAML_BLOCK_SEQUENCE_STYLE);
    yaml_emitter_emit(&emitter, &event);
    yaml_event_delete(&event);  // 删除事件

    // 数组数据，包括新元素
    const char *array[] = {"1", "2", "3", "4", "5"};
    for (int i = 0; i < 5; i++) {
        yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)array[i],
                                     -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
        yaml_emitter_emit(&emitter, &event);
        printf("1\r\n");
        yaml_event_delete(&event);  // 删除事件
        printf("2\r\n");
    }

    // 创建并发射序列结束事件
    yaml_sequence_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);
    yaml_event_delete(&event);  // 删除事件

    // 创建并发射映射结束事件
    yaml_mapping_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);
    yaml_event_delete(&event);  // 删除事件

    // 创建并发射文档结束事件
    yaml_document_end_event_initialize(&event, 0);
    yaml_emitter_emit(&emitter, &event);
    yaml_event_delete(&event);  // 删除事件

    // 创建并发射流结束事件
    yaml_stream_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);
    yaml_event_delete(&event);  // 删除事件

    // 清理发射器
    yaml_emitter_delete(&emitter);

    // 关闭文件
    fclose(file);
}

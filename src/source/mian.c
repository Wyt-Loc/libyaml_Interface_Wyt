/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-07-19 00:10:01
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-08-07 23:31:03
 * @FilePath: /test/src/source/mian.c
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */

#include <stdio.h>
#include "../include/main.h"


int writefile(void){

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

    // 创建并发射映射开始事件
    yaml_mapping_start_event_initialize(&event, NULL, NULL, 0, YAML_BLOCK_MAPPING_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // 创建并发射标量事件 (键)
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"greeting",
                                 -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // 创建并发射标量事件 (值)
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"Hello, World!",
                                 -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
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


int readfile(){

     FILE *file;
    yaml_parser_t parser;
    yaml_event_t event;

    // 打开文件进行读取
    file = fopen("output.yaml", "r");
    if (file == NULL) {
        fputs("Failed to open file for reading!\n", stderr);
        return 1;
    }

    // 初始化解析器
    if (!yaml_parser_initialize(&parser)) {
        fputs("Failed to initialize parser!\n", stderr);
        fclose(file);
        return 1;
    }

    // 设置输入到解析器
    yaml_parser_set_input_file(&parser, file);

    // 解析文档
    while (1) {
        if (!yaml_parser_parse(&parser, &event)) {
            fprintf(stderr, "Parser error %d\n", parser.error);
            return 1;
        }

        // 打印事件类型
        switch (event.type) {
            case YAML_STREAM_START_EVENT:
                printf("Stream start\n");
                break;
            case YAML_STREAM_END_EVENT:
                printf("Stream end\n");
                break;
            case YAML_DOCUMENT_START_EVENT:
                printf("Document start\n");
                break;
            case YAML_DOCUMENT_END_EVENT:
                printf("Document end\n");
                break;
            case YAML_MAPPING_START_EVENT:
                printf("Mapping start\n");
                break;
            case YAML_MAPPING_END_EVENT:
                printf("Mapping end\n");
                break;
            case YAML_SCALAR_EVENT:
                printf("Scalar: %s\n", event.data.scalar.value);
                break;
            default:
                // 其他事件类型不做处理
                break;
        }

        // 检查是否为流结束事件
        if (event.type == YAML_STREAM_END_EVENT) {
            break;
        }

        // 删除事件
        yaml_event_delete(&event);
    }

    // 清理解析器
    yaml_parser_delete(&parser);

    // 关闭文件
    fclose(file);

    return 0;

}


int writearray(void){

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

int writeTest(){

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

    // 创建并发射映射开始事件 (顶级映射)
    yaml_mapping_start_event_initialize(&event, NULL, NULL, 0, YAML_BLOCK_MAPPING_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // 创建并发射标量事件 (键)
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"A",
                                 -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // 创建并发射序列开始事件 (值)
    yaml_sequence_start_event_initialize(&event, NULL, NULL, 0, YAML_BLOCK_SEQUENCE_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // 数组数据
    const char *array[] = {"1", "2", "3", "4"};
    for (int i = 0; i < 4; i++) {
        yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)array[i],
                                     -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
        yaml_emitter_emit(&emitter, &event);
    }

    // 创建并发射序列结束事件
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


int main() {

    // writefile();
    // printf("readOne\r\n");
    // readfile();

    // writearray();
    // printf("readTwo\r\n");
    // readfile();

    // writeTest();
    // printf("readThree\r\n");
    readfile();

}


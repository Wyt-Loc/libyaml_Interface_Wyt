/*
 * @Author: Wyt 1697556601@qq.com
 * @Date: 2024-08-09 23:49:37
 * @LastEditors: Wyt 1697556601@qq.com
 * @LastEditTime: 2024-08-10 13:05:32
 * @FilePath: /libyaml_Interface_Wyt/src/source/readYaml.c
 * @Description: 
 * 
 * Copyright (c) 2024 by Wyt, All Rights Reserved. 
 */


#include <stdio.h>
#include "../include/readYaml.h"


/**
 * @description:  读取YAMl数据
 * @return {*}
 */
int readYamlFile(const char* fileName, const char* openMode){
    yaml_parser_t parser;
    yaml_event_t event;
    FILE* file;

    // 打开文件进行读取
    file = fopen(fileName, openMode);
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

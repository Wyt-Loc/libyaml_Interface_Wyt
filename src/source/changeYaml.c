
#include <yaml.h>
#include <stdio.h>
#include <stdlib.h>

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

int main() {
    FILE *input_file;
    yaml_parser_t parser;
    yaml_event_t event;

    // 打开输入文件进行读取
    input_file = fopen("input.yaml", "r");
    if (input_file == NULL) {
        fputs("Failed to open input file for reading!\n", stderr);
        return 1;
    }

    // 初始化解析器
    if (!yaml_parser_initialize(&parser)) {
        fputs("Failed to initialize parser!\n", stderr);
        fclose(input_file);
        return 1;
    }

    // 设置输入到解析器
    yaml_parser_set_input_file(&parser, input_file);

    // 解析文档并检查事件类型，直到找到需要修改的部分
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
            yaml_event_delete(&event);
            break;
        }

        // 删除事件
        yaml_event_delete(&event);
    }

    // 清理解析器
    yaml_parser_delete(&parser);

    // 关闭输入文件
    fclose(input_file);

    // 打开输出文件进行写入
    FILE *output_file = fopen("output.yaml", "w");
    if (output_file == NULL) {
        fputs("Failed to open output file for writing!\n", stderr);
        return 1;
    }

    // 写入新的数据到输出文件
    write_yaml_with_new_data(output_file);

    return 0;
}

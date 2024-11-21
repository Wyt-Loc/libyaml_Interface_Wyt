#ifndef READYAML_H
#define READYAML_H

#include "yaml.h"


// 定义一个结构体来保存结果
typedef struct {
    float **values;  // 保存值的动态数组
    size_t count;   // 记录获取到的值的数量
} Result;


int readYamlFile(const char* fileName, const char* openMode);
void traverse_yaml_node(yaml_document_t *document, yaml_node_t *node, const char *root, const char *sub_key, const char *key, Result *result, int *found_root, int *found_sub_key, int *found_key, int *found);
Result getValueFromYaml(const char *filename, const char *root, const char *sub_key, const char *key);
void free_result(Result *result);


#endif
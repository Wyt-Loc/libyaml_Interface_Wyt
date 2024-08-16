/*
 * @Author: Wyt 1697556601@qq.com
 * @Date: 2024-08-08 00:32:23
 * @LastEditors: Wyt 1697556601@qq.com
 * @LastEditTime: 2024-08-11 15:15:52
 * @FilePath: /libyaml_Interface_Wyt/src/include/writeYaml.h
 * @Description: 
 * 
 * Copyright (c) 2024 by Wyt, All Rights Reserved. 
 */
#ifndef WRITEYAML_H
#define WRITEYAML_H

#include "yaml.h"

typedef struct{

    char *keyName;    // 键名
    char *value;    // 指向字符串数组的指针

}YamlData;


YamlData* createKeyValue(const char *keyName, const char *Value);

int writeYamlFile(const char* fileName, const char* openMode, YamlData* yamldata);


#endif
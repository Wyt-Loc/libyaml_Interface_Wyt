/*
 * @Author: Wyt 1697556601@qq.com
 * @Date: 2024-08-08 00:32:23
 * @LastEditors: Wyt 1697556601@qq.com
 * @LastEditTime: 2024-08-13 22:33:48
 * @FilePath: /libyaml_Interface_Wyt/src/include/writeArray.h
 * @Description: 
 * 
 * Copyright (c) 2024 by Wyt, All Rights Reserved. 
 */
#ifndef WRITEARRAY_H
#define WRITEARRAY_H

#include "yaml.h"


// 此结构体用来存储要写入的键值对序列也就是 sequence 格式数据
typedef struct {  
    char *keyName;    // 键名
    char **values;    // 指向字符串数组的指针
    int numValues; // 键值数组的长度  
} KeyValueStore;

KeyValueStore* createKeyValueStore(const char *keyName, int numValues);
void addValue(KeyValueStore *store, int index, const char *value);
void freeKeyValueStore(KeyValueStore *store);


int writeYamlArray(const  char* fileName, const char* openMode, KeyValueStore *store);
int writeYamlDepthTwo(void);
int readDataaa(void);
#endif


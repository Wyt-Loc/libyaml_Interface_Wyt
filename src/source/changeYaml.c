/*
 * @Author: Wyt 1697556601@qq.com
 * @Date: 2024-08-08 00:32:23
 * @LastEditors: Wyt 1697556601@qq.com
 * @LastEditTime: 2024-08-10 13:07:00
 * @FilePath: /libyaml_Interface_Wyt/src/source/changeYaml.c
 * @Description: 
 * 
 * Copyright (c) 2024 by Wyt, All Rights Reserved. 
 */


#include <stdio.h>

#include "../include/changeYaml.h"


void write_yaml_with_new_data(const char* fileName, const char* openMode) {

    FILE* file;

    // 打开文件进行读取
    file = fopen(fileName, openMode);
    if (file == NULL) {
        fputs("Failed to open file for reading!\n", stderr);
        return ;
    }

}

/*
 * @Author: Wyt 1697556601@qq.com
 * @Date: 2024-08-10 14:57:41
 * @LastEditors: Wyt 1697556601@qq.com
 * @LastEditTime: 2024-08-10 21:05:48
 * @FilePath: /libyaml_Interface_Wyt/src/include/getAllData.h
 * @Description: 
 * 
 * Copyright (c) 2024 by Wyt, All Rights Reserved. 
 */

#ifndef GETALLDATA_H
#define GETALLDATA_H

#include <yaml.h>
#include <stdio.h>


void traverse_yaml_node(yaml_document_t *document, yaml_node_t *node);
int getAllData(void);


#endif
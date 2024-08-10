/*
 * @Author: Wyt 1697556601@qq.com
 * @Date: 2024-08-10 13:36:04
 * @LastEditors: Wyt 1697556601@qq.com
 * @LastEditTime: 2024-08-10 13:37:50
 * @FilePath: /libyaml_Interface_Wyt/src/include/getYamlRootNode.h
 * @Description: 
 * 
 * Copyright (c) 2024 by Wyt, All Rights Reserved. 
 */

#ifndef GETYAMLROOTNODE_H
#define GETYAMLROOTNODE_H

#include <yaml.h>
#include <stdio.h>


int getYamlFileRootNode(const char* fileName, const char* openMode);


#endif // GETYAMLROOTNODE_H
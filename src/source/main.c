/*
 * @Author: Wyt 1697556601@qq.com
 * @Date: 2024-08-10 01:32:02
 * @LastEditors: Wyt 1697556601@qq.com
 * @LastEditTime: 2024-08-20 23:44:45
 * @FilePath: /libyaml_Interface_Wyt/src/source/main.c
 * @Description: 
 * 
 * Copyright (c) 2024 by Wyt, All Rights Reserved. 
 */


#include <stdio.h>

#include "../include/main.h"
#include "../include/readYaml.h"
#include "../include/writeYaml.h"
#include "../include/writeArray.h"
#include "../include/getAllData.h"
#include "../include/getYamlRootNode.h"
#include "../include/IterativeMapping.h"


int main(void){

    const char *inPutFileName = "../../yamlfile/input.yaml";

    YamlData* yamldata = createKeyValue("Key0", "Value0");

    // 创建要写入的键值对。
    KeyValueStore* store = createKeyValueStore("TestKey1", 3);
    addValue(store, 0, "value1");
    addValue(store, 1, "value2");
    addValue(store, 2, "value3");

    // 写入文件
    writeYamlFile(inPutFileName, "w", yamldata);
    writeYamlArray(inPutFileName, "a", store);
    writeYamlDepthTwo();
    // 得到所有类型的数据
    getAllData();

    //freeKeyValueStore(store);
    return 0;

}

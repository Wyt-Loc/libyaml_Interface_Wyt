## Welcome to this repository

#### The environment and software version used are:

![tip](https://badgen.net/badge/Ubuntu/22.04LTS/black?icon=packagephobia) 		![tip](https://badgen.net/badge/C/201710L-c17/green?icon=packagephobia) 		![tip](https://badgen.net/badge/libyaml/2.0.9/red?icon=github) 		![tip](https://badgen.net/badge/GCC/11.40/orange?icon=github)

| Ubuntu 22.04.4 LTS |
| ------------------ |
| GCC version 11.4.0 |
| libyaml-2.0.9      |

基于C语言的libyaml库进行封装完善工作。主要是进行接口函数的编写。

src目录: 主要存放接口代码。

​	- source目录：接口代码的源文件目录。

​	- include目录：接口代码的头文件目录

​		- debug目录：调试常用的系统及头文件目录方便调用所设此目录。

target目录：GCC编译的过程文件以及可执行文件的存放目录。

yamlfile目录：用来存放yaml文件的目录。

![image-20240810235018789](./image-20240810235018789.png)


写入键值:
![image-20240810235018789](./image-writeyaml.png)

写入数组:
![image-20240810235018789](./image-writeArray.png)

写入序列:


读取:
![image-20240810235018789](./image-readYaml.png)
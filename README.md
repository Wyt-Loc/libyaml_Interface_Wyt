## Welcome to this repository

<a href="https://info.flagcounter.com/vhOC"><img src="https://s01.flagcounter.com/count2/vhOC/bg_FFFFFF/txt_000000/border_CCCCCC/columns_2/maxflags_10/viewers_0/labels_0/pageviews_0/flags_0/percent_0/" alt="Flag Counter" border="0"></a>

<img href="https://codetime.dev" alt="CodeTime Badge" src="https://img.shields.io/endpoint?style=flat-square&color=green&url=https%3A%2F%2Fapi.codetime.dev%2Fshield%3Fid%3D26798%26project%3Dlibyaml_Interface_Wyt [SSH: 192.168.124.3]%26in=0">

#### The environment and software version used are:

![tip](https://badgen.net/badge/Ubuntu/22.04LTS/black?icon=packagephobia) 		![tip](https://badgen.net/badge/C/201710L-c17/green?icon=packagephobia) 		![tip](https://badgen.net/badge/libyaml/2.0.9/red?icon=github) 		![tip](https://badgen.net/badge/GCC/11.40/orange?icon=github)

| **Ubuntu 22.04.4 LTS** |
| ------------------ |
| **GCC version 11.4.0** |
| **libyaml-2.0.9**      |

基于C语言的libyaml库进行封装完善工作。主要是进行接口函数的编写。

src目录: 主要存放接口代码。

​	- source目录：接口代码的源文件目录。

​	- include目录：接口代码的头文件目录

​		- debug目录：调试常用的系统及头文件目录方便调用所设此目录。

target目录：GCC编译的过程文件以及可执行文件的存放目录。

yamlfile目录：用来存放yaml文件的目录。

### 执行方式(method of application)

```bash
cd src/source
sudo chmod  +x  app_yaml_start.sh
./app_yaml_start.sh
```

![image-20240810235018789](./image-20240810235018789.png)

写入键值:
![image-20240810235018789](./image-writeyaml.png)

写入数组:
![image-20240810235018789](./image-writeArray.png)

写入序列:

读取:
![image-20240810235018789](./image-readYaml.png)

## 鸣谢

感谢libyaml官方的开源. [libyaml](https://github.com/yaml/libyaml)

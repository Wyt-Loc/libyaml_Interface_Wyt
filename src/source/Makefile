CC=gcc

# 定义编译选项，包括头文件目录
# 启用大多数警告信息， 启用额外警告信息，根据 ISO C 标准来检查代码，头文件路径添加
CFLAGS=-Wall -Wextra -pedantic -I/../include/

# 定义链接选项，包括库目录和库名
LDFLAGS=-lyaml

# 定义源文件和对象文件列表（不带路径）
# SOURCES=main.c readYaml.c writeYaml.c writeArray.c changeYaml.c
SOURCES = $(wildcard *.c)
# 定义目标文件
OBJECTS=$(SOURCES:.c=.o) 

# 定义目标目录
TARGET_DIR=../../target

# 定义最终的可执行文件（带路径）
TARGET=$(TARGET_DIR)/main

# 定义对象文件的完整路径（带target目录）
OBJECT_FILES=$(addprefix $(TARGET_DIR)/, $(OBJECTS))

# 默认目标：编译所有源文件并链接生成可执行文件  
all: $(TARGET_DIR) $(TARGET)  

# 链接目标文件生成最终的可执行文件  
$(TARGET):$(OBJECT_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# 编译每个源文件到对应的对象文件（放在target目录下） 加上@ 不显示链接信息
$(TARGET_DIR)/%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@  

# 清理编译生成的文件
clean:  
	rm -rf $(TARGET_DIR)/*

# 定义对象文件变量
obj := bin/hello.o bin/main.o bin/add.o
CC	:= gcc
BIN_DIR := bin

# main目标依赖于对象文件列表
main:$(obj)
	$(CC) $(obj) -o main

# main.o目标依赖于main.c和hello.h
bin/main.o: main.c hello.h | $(BIN_DIR)
	$(CC) -c main.c -o  $(BIN_DIR)/main.o

# hello.o目标依赖于hello.c和hello.h
bin/hello.o: hello.c hello.h | $(BIN_DIR)
	$(CC) -c hello.c -o  $(BIN_DIR)/hello.o

# add.o目标依赖于add.c和hello.h
bin/add.o: add.c hello.h | $(BIN_DIR)
	$(CC) -c add.c -o  $(BIN_DIR)/add.o

# 创建 bin 目录
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# clean目标用于清理生成的文件
clean:
	-rm -rf $(BIN_DIR)	main


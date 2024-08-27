




CC := gcc
CFLAGS := -Wall -O2
TARGET := main
SRC_DIR := src
INL_DIR := include
BIN_DIR := bin

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))

VPATH := $(SRC_DIR):$(INL_DIR)

$(BIN_DIR)/$(TARGET): $(OBJ_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) -o $@ $(OBJ_FILES) -I$(INL_DIR)

$(OBJ_FILES): $(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BIN_DIR)
	$(CC) -c $< -o $@ -I$(INL_DIR)


.PHONY : clean
clean:
	rm -rf $(BIN_DIR)


# CC      := gcc
# CFLAGS = -Wall -O2
# TARGET  := main
# BIN_DIR := bin
# SRC_DIR := src
# INC_DIR := include

# # VPATH 设置
# VPATH := $(SRC_DIR):$(INC_DIR)

# # 获取 src 目录下的所有 .c 文件并转换为 .o 文件
# SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
# OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))

# # 目标生成规则
# $(BIN_DIR)/$(TARGET): $(OBJ_FILES)
# 	mkdir -p $(BIN_DIR)
# 	$(CC) -o $@ $(OBJ_FILES) -I$(INC_DIR)

# # 每个 .c 文件的编译规则
# $(OBJ_FILES): $(BIN_DIR)/%.o: $(SRC_DIR)/%.c
# 	mkdir -p $(BIN_DIR)
# 	$(CC) -c $< -o $@ -I$(INC_DIR)


# .PHONY: clean
# clean:
# 	-rm -rf $(BIN_DIR)




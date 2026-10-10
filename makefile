CC = gcc
CFLAGS = -Wall -Wextra -g
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
TARGET = $(BIN_DIR)/myshell
SRCS = $(wildcard $(SRC_DIR)/*.c)
HDRS = $(wildcard $(INC_DIR)/*.h)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all run clean

all: $(TARGET)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
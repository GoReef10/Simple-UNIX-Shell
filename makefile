CC = gcc
CFLAGS = -Wall -Wextra -g
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TARGET = $(BIN_DIR)/myshell.exe
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*
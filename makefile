CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = myshell.exe
SRCS = shell.c parser.c main.c execute_commands.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
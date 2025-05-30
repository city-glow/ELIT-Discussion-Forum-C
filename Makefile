# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# OpenSSL linking
LIBS = -lssl -lcrypto

# Source files
SRCS = src/main.c src/user/user.c src/user/userList.c src/auth/auth.c src/ui/ui.c src/board/board.c src/board/boardList.c src/post/post.c src/post/postList.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output binary
TARGET = app

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean compiled files
clean:
	rm -f $(OBJS) $(TARGET)

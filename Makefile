# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g -I"C:/msys64/mingw64/include" -I"C:/msys64/mingw64/include/openssl"

# OpenSSL linking
LIBS = -L"C:/msys64/mingw64/lib" -lssl -lcrypto

# Source files
SRCS = src/user/user.c src/user/userList.c src/auth/auth.c src/ui/ui.c src/board/board.c src/board/boardList.c src/post/post.c src/post/postList.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output binary
TARGET = app

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -mconsole $(LIBS)

# Compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean compiled files
clean:
ifeq ($(OS), Windows_NT)
	@if exist src\user\user.o cmd /C del /F /Q src\user\user.o
	@if exist src\user\userList.o cmd /C del /F /Q src\user\userList.o
	@if exist src\auth\auth.o cmd /C del /F /Q src\auth\auth.o
	@if exist src\ui\ui.o cmd /C del /F /Q src\ui\ui.o
	@if exist src\board\board.o cmd /C del /F /Q src\board\board.o
	@if exist src\board\boardList.o cmd /C del /F /Q src\board\boardList.o
	@if exist src\post\post.o cmd /C del /F /Q src\post\post.o
	@if exist src\post\postList.o cmd /C del /F /Q src\post\postList.o
	@if exist $(TARGET) cmd /C del /F /Q $(TARGET)
else
	rm -f $(OBJS) $(TARGET)
endif
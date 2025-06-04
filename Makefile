# Compiler
CC = gcc

# Detect OS
ifeq ($(OS),Windows_NT)
	# Windows settings
	CFLAGS = -Wall -Wextra -g -I"C:/msys64/mingw64/include" -I"C:/msys64/mingw64/include/openssl"
	LIBS = -L"C:/msys64/mingw64/lib" -lssl -lcrypto
	DEL = cmd /C del /F /Q
	SEP = \\
else
	# Linux/macOS settings
	CFLAGS = -Wall -Wextra -g
	LIBS = -lssl -lcrypto
	DEL = rm -f
	SEP = /
endif

# Target executable
TARGET = app

# Source files
SRCS = \
	src/user/user.c \
	src/user/userList.c \
	src/auth/auth.c \
	src/ui/ui.c \
	src/board/board.c \
	src/board/boardList.c \
	src/post/post.c \
	src/post/postList.c \
	src/vote/vote.c \
	src/vote/voteList.c \
	src/main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

# Compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
ifeq ($(OS),Windows_NT)
	$(DEL) src\user\*.o
	$(DEL) src\auth\*.o
	$(DEL) src\ui\*.o
	$(DEL) src\board\*.o
	$(DEL) src\vote\*.o
	$(DEL) src\post\*.o
	$(DEL) src\main.o
	$(DEL) $(TARGET).exe
else
	$(DEL) $(OBJS) $(TARGET)
endif

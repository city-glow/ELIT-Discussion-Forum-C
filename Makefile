# Compiler
CC = gcc

# Detect OS
ifeq ($(OS),Windows_NT)
	# Windows settings
	CFLAGS = -Wall -Wextra -g
	LIBS = -L"C:/msys64/mingw64/lib" -lssl -lcrypto -largon2
	DEL = cmd /C del /F /Q
	SEP = \\
else
	# Linux/macOS settings
	CFLAGS = -Wall -Wextra -g
	LIBS = -lssl -lcrypto -largon2
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
	src/comment/comment.c \
	src/comment/commentTree.c \
	src/comment/commentTreeList.c \
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
	@cmd /C del /F /Q src\user\*.o 2> nul
	@cmd /C del /F /Q src\auth\*.o 2> nul
	@cmd /C del /F /Q src\ui\*.o 2> nul
	@cmd /C del /F /Q src\board\*.o 2> nul
	@cmd /C del /F /Q src\vote\*.o 2> nul
	@cmd /C del /F /Q src\post\*.o 2> nul
	@cmd /C del /F /Q src\main.o 2> nul
	@cmd /C del /F /Q $(TARGET).exe 2> nul
else
	$(DEL) $(OBJS) $(TARGET)
endif

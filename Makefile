# Compiler
CC = gcc

# Detect OS
ifeq ($(OS),Windows_NT)
	# Windows settings
	CFLAGS = -Wall -Wextra -g -Iinclude/phc-winner-argon2/include
	LIBS =
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
ifeq ($(OS),Windows_NT)
	SRCS = \
		src/user/user.c \
		src/user/userList.c \
		src/auth/auth.c \
		src/ui/ui.c \
		src/board/board.c \
		src/board/boardList.c \
		src/board/moderateThisList.c \
		src/board/moderateThisQueue.c \
		src/post/post.c \
		src/post/postList.c \
		src/comment/comment.c \
		src/comment/commentTree.c \
		src/comment/commentTreeList.c \
		src/vote/vote.c \
		src/vote/voteList.c \
		src/array/item.c \
		src/array/itemArray.c \
		include/phc-winner-argon2/src/argon2.c \
		include/phc-winner-argon2/src/core.c \
		include/phc-winner-argon2/src/encoding.c \
		include/phc-winner-argon2/src/blake2/blake2b.c \
		include/phc-winner-argon2/src/thread.c \
		include/phc-winner-argon2/src/ref.c \
		src/main.c
else
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
		src/array/item.c \
		src/array/itemArray.c \
		src/id/id.c \
		src/id/idList.c \
		src/board/moderateThisList.c \
		src/board/moderateThisQueue.c \
		src/main.c
endif
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

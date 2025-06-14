# Compiler
CC = gcc

# Detect OS
# Detect OS
ifeq ($(OS),Windows_NT)
    CFLAGS = -Wall -Wextra -g -Iinclude/phc-winner-argon2/include
    LIBS =
    DEL = cmd /C del /F /Q
    SEP = \\
    MKDIR_P = powershell -Command "New-Item -ItemType Directory -Force -Path '$(subst /,\\,$(1))' >$null"
else
    CFLAGS = -Wall -Wextra -g
    LIBS = -largon2
    DEL = rm -f
    SEP = /
    MKDIR_P = mkdir -p $(1)
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
		src/id/id.c \
		src/id/idList.c \
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

OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))


# Default target
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	$(call MKDIR,$(BUILD_DIR))


# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $(OBJS) $(LIBS)

# Compilation
$(BUILD_DIR)/%.o: %.c
	@$(call MKDIR_P,$(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@



# Clean
clean:
ifeq ($(OS),Windows_NT)
	@if exist "$(BUILD_DIR)" powershell -Command "Remove-Item -Recurse -Force '$(BUILD_DIR)'"
	@if exist "$(TARGET).exe" del /F /Q $(TARGET).exe
else
	$(DEL) -r $(BUILD_DIR) $(BIN_DIR)/$(TARGET)
endif

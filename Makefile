# Compiler
CC = gcc

# Detect OS
# Detect OS
ifeq ($(OS),Windows_NT)
    CFLAGS = -Wall -Wextra -g -Iinclude/argon2
	LIBS = 
    DEL = cmd /C del /F /Q
    SEP = \\
else
    CFLAGS = -Wall -Wextra -g
    LIBS = -largon2
    DEL = rm -f
    SEP = /
endif

ifeq ($(OS),Windows_NT)
define MKDIR_P
powershell -NoProfile -Command "New-Item -ItemType Directory -Force -Path '$(subst /,\\,$1)' | Out-Null"
endef

else
define MKDIR_P
mkdir -p $1
endef
endif

# Target executable
TARGET = app

# Source files
SRCS = \
	src/user/user.c \
	src/user/userList.c \
	src/auth/auth.c \
	src/ui/ui.c \
	src/ui/navigationStack.c \
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
	src/board/moderateThisList.c \
	src/board/moderateThisQueue.c \
	src/argon2/argon2.c \
	src/argon2/core.c \
	src/argon2/encoding.c \
	src/argon2/blake2/blake2b.c \
	src/argon2/thread.c \
	src/argon2/ref.c \
	src/main.c

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
ifeq ($(OS),Windows_NT)
	@copy libs\libargon2.dll $(BIN_DIR)\libargon2.dll >nul
endif



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

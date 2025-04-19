# Project Structure
SRC_DIR   := src
OBJ_DIR   := obj
BIN_DIR   := bin
LIB_DIR   := lib

# Compiler and Flags
CC        := gcc
CFLAGS    := -Wall -Wextra -I$(SRC_DIR)
DEBUG     := -g
FAST      := -Ofast -march=native -unroll-loops

# Targets
TARGET    := $(BIN_DIR)/huffman
SRCS      := $(wildcard $(SRC_DIR)/*.c)
OBJS      := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS      := $(OBJS:.o=.d)  # Auto-generated dependencies

# Default: Debug build
all: CFLAGS += $(DEBUG)
all: $(TARGET)

# fast build
fast: CFLAGS += $(FAST)
fast: $(TARGET)

# Link executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Compile sources + generate dependencies
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Create directories if missing
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Include auto-generated dependencies
-include $(DEPS)

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean

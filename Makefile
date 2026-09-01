CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -g3 -fsanitize=address,undefined -Iinclude
LDFLAGS = -fsanitize=address,undefined

# Directories
SRC_DIR = src
INC_DIR = include
TEST_DIR = tests
BUILD_DIR = build

.PHONY: all clean test

# 1. Default target (what 'make' runs by default)
all: test

# 2. Test target (build and run test binaries)
test: $(BUILD_DIR)/test_kv_string
	  ./$(BUILD_DIR)/test_kv_string

$(BUILD_DIR)/test_kv_string: $(TEST_DIR)/test_kv_string.c $(SRC_DIR)/kv_string.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


# 3. Clean target (remove build artifacts)
clean:
	rm -rf $(BUILD_DIR)

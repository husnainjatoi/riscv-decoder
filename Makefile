CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC_DIR   = src
BUILD_DIR = build
BIN_DIR   = bin
TEST_DIR  = test

TARGET      = $(BIN_DIR)/riscv-decoder
TEST_TARGET = $(BIN_DIR)/test-decoder

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/memory.c $(SRC_DIR)/decoder.c
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/memory.o $(BUILD_DIR)/decoder.o

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS += -O0
debug: all
	@echo "Build completed in explicit debug environment (-O0)."

test: $(TEST_DIR)/test_decoder.c $(SRC_DIR)/decoder.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(TEST_DIR)/test_decoder.c $(SRC_DIR)/decoder.c -o $(TEST_TARGET)
	@echo "Executing automated SQA unit tests..."
	./$(TEST_TARGET)

valgrind: all
	@echo "Initiating dynamic memory leak analysis..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) test/programs/mixed.hex

clean:
	@echo "Purging temporary object artifacts and binaries..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)

help:
	@echo "========================================================================"
	@echo " Available commands:"
	@echo "  make             Compile and build the production binary (default)"
	@echo "  make debug       Build executable with disabled compiler optimizations (-O0)"
	@echo "  make test        Compile and run the automated SQA unit test suite"
	@echo "  make valgrind    Compile code and run memory leak diagnostic check"
	@echo "  make clean       Purge all compiled binaries, objects, and build folders"
	@echo "  make help        Display this interactive documentation summary"
	@echo "========================================================================"

.PHONY: all clean test debug valgrind help
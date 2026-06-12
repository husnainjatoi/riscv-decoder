CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

# Directory configuration for source, object files, and binaries
SRC_DIR   = src
BUILD_DIR = build
BIN_DIR   = bin
TEST_DIR  = test

TARGET      = $(BIN_DIR)/riscv-decoder
TEST_TARGET = $(BIN_DIR)/test-decoder

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/memory.c $(SRC_DIR)/decoder.c
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/memory.o $(BUILD_DIR)/decoder.o

# Default target: builds the main production executable
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Generic rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Builds without optimization for easier debugging
debug: CFLAGS += -O0
debug: all
	@echo "Build completed in explicit debug environment (-O0)."

# Compiles and runs the SQA unit test suite
test: $(TEST_DIR)/test_decoder.c $(SRC_DIR)/decoder.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(TEST_DIR)/test_decoder.c $(SRC_DIR)/decoder.c -o $(TEST_TARGET)
	@echo "Executing automated SQA unit tests..."
	./$(TEST_TARGET)

# Run the decoder on a specified hex file (defaults to mixed.hex)
FILE ?= test/programs/mixed.hex

run: all
	@echo "Executing decoder on $(FILE)..."
	@echo "--------------------------------------------------------"
	@./$(TARGET) $(FILE)

# Runs Valgrind to ensure zero memory leaks
valgrind: all
	@echo "Initiating dynamic memory leak analysis..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) test/programs/mixed.hex

# Cleans up build artifacts and output binaries
clean:
	@echo "Purging temporary object artifacts and binaries..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Displays help documentation
help:
	@echo "========================================================================"
	@echo " RISC-V RV32I Instruction Decoder Automation Layer Help"
	@echo "========================================================================"
	@echo " Available commands:"
	@echo "  make             Compile and build the production binary (default)"
	@echo "  make debug       Build executable with disabled compiler optimizations (-O0)"
	@echo "  make test        Compile and run the automated SQA unit test suite"
	@echo "  make valgrind    Compile code and run memory leak diagnostic check"
	@echo "  make clean       Purge all compiled binaries, objects, and build folders"
	@echo "  make help        Display this interactive documentation summary"
	@echo "  make run         Compile and execute the decoder (defaults to mixed.hex)"
	@echo "                   Usage: make run [FILE=path/to/custom.hex]"    
	@echo "========================================================================"

.PHONY: all clean test debug valgrind help run
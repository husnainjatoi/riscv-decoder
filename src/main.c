#include <stdio.h>
#include <stdlib.h>
#include "../include/memory.h"
#include "../include/decoder.h"

int main(int argc, char *argv[]) {
    // Validate command-line arguments to ensure a file path is provided.
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path_to_hex_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* filename = argv[1];
    int num_instructions = 0;

    // Load instructions from the hex file into dynamically allocated memory.
    uint32_t* memory = load_hex_file(filename, &num_instructions);
    
    // Check if the memory load was successful.
    if (memory == NULL || num_instructions == 0) {
        fprintf(stderr, "Error: Failed to load instructions from %s\n", filename);

        if (memory != NULL) {
            free(memory); // Prevent memory leak on failure.
        }
        return EXIT_FAILURE;
    }

    printf("\nRISC-V RV32I Instruction Decoder\n");
    printf("========================================================\n\n"); 
    printf("Loaded %d instructions from %s\n\n", num_instructions, filename);
    
    printf("%-11s     %-11s %s\n", "Addr", "Hex", "Assembly");
    printf("--------------------------------------------------------\n"); 

    int valid_count = 0;
    int unknown_count = 0;

    // Iterate through the loaded instructions, decoding and printing them one by one.
    for (int i = 0; i < num_instructions; i++) {
        uint32_t current_inst = memory[i];
        uint32_t current_address = i * 4;

        decodedinst_t decoded;
        decode_instruction(current_inst, &decoded);
        print_assembly(current_address, current_inst, &decoded);

        // Track valid vs unknown instructions for the final summary.
        if (decoded.opcode == OP_UNKNOWN) {
            unknown_count++;
        } else {
            valid_count++;
        }
    }

    printf("\nDecoded %d instructions (%d valid, %d unknown)\n", num_instructions, valid_count, unknown_count);

    free(memory);

    return EXIT_SUCCESS;
}
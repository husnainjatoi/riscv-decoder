#include <stdio.h>
#include <stdlib.h>
#include "../include/memory.h"
#include "../include/decoder.h"

int main(int argc, char *argv[]) {
 
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path_to_hex_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* filename = argv[1];
    int num_instructions = 0;

    uint32_t* memory = load_hex_file(filename, &num_instructions);
    
    if (memory == NULL || num_instructions == 0) {
        fprintf(stderr, "Error: Failed to load instructions from %s\n", filename);

        if (memory != NULL) {
            free(memory);
        }
        return EXIT_FAILURE;
    }

    printf("RISC-V RV32I Instruction Decoder\n");
    printf("Loaded %d instructions from %s\n", num_instructions, filename);
    printf("%-10s  %-10s  %s\n", "Addr", "Hex", "Assembly");
    printf("--------------------------------------------------------\n"); 

    int valid_count = 0;
    int unknown_count = 0;

    for (int i = 0; i < num_instructions; i++) {
        uint32_t current_inst = memory[i];
        
        uint32_t current_address = i * 4; 

        decodedinst_t decoded;
        decode_instruction(current_inst, &decoded);
        
        print_assembly(current_address, current_inst, &decoded);

        if (decoded.opcode == OP_UNKNOWN) {
            unknown_count++;
        } else {
            valid_count++;
        }
    }

    printf("--------------------------------------------------------\n");
    printf("Decoded %d instructions (%d valid, %d unknown)\n", num_instructions, valid_count, unknown_count);

    free(memory);

    return EXIT_SUCCESS;
}
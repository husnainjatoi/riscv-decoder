#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/memory.h"

#define INITIAL_CAPACITY 64
#define MAX_LINE_CAPACITY 256

uint32_t *load_hex_file(const char* filename, int *num_instructions){
    FILE *file = fopen(filename, "r");
    if(!file){
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        *num_instructions = 0;
        return NULL;
    }

    int capacity = INITIAL_CAPACITY;
    int count = 0;

    uint32_t *memory = (uint32_t*) malloc (capacity * sizeof(uint32_t));

    if(!memory){
        fprintf(stderr, "Error: Initial memory allocation failed!\n");
        fclose(file);
        *num_instructions = 0;
        return NULL;
    }

    char line[MAX_LINE_CAPACITY];
    
    while(fgets(line, sizeof(line), file)){
        if(line[0] == '\n' || line[0] == '\r' || line[0] == '\0'){
            continue;
        }

        if(count >= capacity){

            capacity *= 2;
            uint32_t *temp = (uint32_t *)realloc(memory, capacity * sizeof(uint32_t));
            if(!temp){
                fprintf(stderr, "Error: Memory allocation failed at instruction %d\n", count);
                free(memory);
                fclose(file);
                *num_instructions = 0;
                return NULL;
            }
            memory = temp;
        }

    memory[count] = (uint32_t) strtoul (line, NULL, 16);
    count ++;

    }

    fclose(file);
    *num_instructions = count;

    return memory;
}
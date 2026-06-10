#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

uint32_t *load_hex_file(const char *filename, int *num_instructions);

#endif
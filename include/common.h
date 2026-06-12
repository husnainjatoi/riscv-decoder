#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

// Extracts a specific range of bits [end:start] from a 32-bit instruction.
// Mimics hardware wire-slicing (e.g., Verilog's inst[end:start]).
#define EXTRACT_BITS(inst, start, end) (((inst) >> (start)) & ((1U << ((end) - (start) + 1)) - 1))

#endif
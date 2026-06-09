#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define EXTRACT_BITS(inst, start, end) (((inst) >> (start)) & ((1U << ((end) - (start) + 1)) - 1))

#endif
#include <stdio.h>
#include "../include/decoder.h"

void decode_instruction(uint32_t inst, DecodedInst_t *decoded) {
    decoded->opcode = (opcode_t)EXTRACT_BITS(inst, 0, 6);

    decoded->rd = 0; decoded->rs1 = 0; decoded->rs2 = 0;
    decoded->funct3 = 0; decoded->funct7 = 0; decoded->imm = 0;

    switch (decoded->opcode) {
        case OP_R_TYPE:
            decoded->rd     = EXTRACT_BITS(inst, 7, 11);
            decoded->funct3 = EXTRACT_BITS(inst, 12, 14);
            decoded->rs1    = EXTRACT_BITS(inst, 15, 19);
            decoded->rs2    = EXTRACT_BITS(inst, 20, 24);
            decoded->funct7 = EXTRACT_BITS(inst, 25, 31);
            break;
            
        default:
            decoded->opcode = OP_UNKNOWN;
            break;
    }
}
#include <stdio.h>
#include "../include/decoder.h"

static int32_t sign_extend(int32_t value, int bit_count) {
    int32_t sign_bit = 1 << (bit_count - 1);
    if (value & sign_bit) {
        value |= (0xFFFFFFFF << bit_count); 
    }
    return value;
}

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
        
        case OP_I_TYPE_ARITH:
        case OP_I_TYPE_LOAD:
        case OP_I_TYPE_JALR:
            decoded->rd     = EXTRACT_BITS(inst, 7, 11);
            decoded->funct3 = EXTRACT_BITS(inst, 12, 14);
            decoded->rs1    = EXTRACT_BITS(inst, 15, 19);
            // I-Type immediate is top 12 bits: inst[31:20]
            decoded->imm    = EXTRACT_BITS(inst, 20, 31);
            decoded->imm    = sign_extend(decoded->imm, 12);
            break;
            
        default:
            decoded->opcode = OP_UNKNOWN;
            break;
    }
}
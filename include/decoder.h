#ifndef DECODER_H
#define DECODER_H

#include "common.h"

typedef enum {
    OP_R_TYPE       = 0x33,
    OP_I_TYPE_ARITH = 0x13,
    OP_I_TYPE_LOAD  = 0x03,
    OP_S_TYPE       = 0x23,
    OP_B_TYPE       = 0x63,
    OP_U_TYPE_LUI   = 0x37,
    OP_U_TYPE_AUIPC = 0x17, 
    OP_J_TYPE_JAL   = 0x6F,
    OP_I_TYPE_JALR  = 0x67, 
    OP_UNKNOWN      = 0x00  // Fallback for invalid instructions
} opcode_t;

typedef struct {
    opcode_t opcode;
    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;
    uint8_t funct3;
    uint8_t funct7;
    int32_t imm;
} decodedinst_t;

void decode_instruction(uint32_t instr, decodedinst_t *decoded);
void print_assembly(uint32_t address, uint32_t inst, const decodedinst_t *decoded);

#endif
#include <stdio.h>
#include "../include/decoder.h"

static int32_t sign_extend(int32_t value, int bit_count) {
    int32_t sign_bit = 1 << (bit_count - 1);
    if (value & sign_bit) {
        value |= ((int32_t)0xFFFFFFFF << bit_count); 
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
            
            decoded->imm    = EXTRACT_BITS(inst, 20, 31);
            decoded->imm    = sign_extend(decoded->imm, 12);
            
            decoded->funct7 = EXTRACT_BITS(inst, 25, 31);
            break;

        case OP_S_TYPE:
            decoded->funct3 = EXTRACT_BITS(inst, 12, 14);
            decoded->rs1    = EXTRACT_BITS(inst, 15, 19);
            decoded->rs2    = EXTRACT_BITS(inst, 20, 24);
            decoded->imm    = (EXTRACT_BITS(inst, 25, 31) << 5) | EXTRACT_BITS(inst, 7, 11);
            decoded->imm    = sign_extend(decoded->imm, 12);
            break;

        case OP_B_TYPE:
            decoded->funct3 = EXTRACT_BITS(inst, 12, 14);
            decoded->rs1    = EXTRACT_BITS(inst, 15, 19);
            decoded->rs2    = EXTRACT_BITS(inst, 20, 24);
            decoded->imm    = (EXTRACT_BITS(inst, 31, 31) << 12) |
                              (EXTRACT_BITS(inst, 7, 7)   << 11) |
                              (EXTRACT_BITS(inst, 25, 30) << 5)  |
                              (EXTRACT_BITS(inst, 8, 11)  << 1);
            decoded->imm    = sign_extend(decoded->imm, 13);
            break;

        case OP_U_TYPE_LUI:
        case OP_U_TYPE_AUIPC:
            decoded->rd     = EXTRACT_BITS(inst, 7, 11);
            decoded->imm    = EXTRACT_BITS(inst, 12, 31) << 12;
            break;

        case OP_J_TYPE_JAL:
            decoded->rd     = EXTRACT_BITS(inst, 7, 11);
            decoded->imm    = (EXTRACT_BITS(inst, 31, 31) << 20) |
                              (EXTRACT_BITS(inst, 12, 19) << 12) |
                              (EXTRACT_BITS(inst, 20, 20) << 11) |
                              (EXTRACT_BITS(inst, 21, 30) << 1);
            decoded->imm    = sign_extend(decoded->imm, 21);
            break;
            
        default:
            decoded->opcode = OP_UNKNOWN;
            break;
    }
}

void print_assembly(uint32_t address, uint32_t inst, const DecodedInst_t *decoded) {
    if (decoded->opcode == OP_UNKNOWN) {
        printf("0x%08x  %08X    UNKNOWN\n", address, inst);
        return;
    }

    char assembly_str[64] = "UNKNOWN";
    const char* mnemonic = "unknown";

    switch (decoded->opcode) {
        case OP_R_TYPE:
            switch (decoded->funct3) {
                case 0x0: mnemonic = (decoded->funct7 == 0x20) ? "sub" : "add"; break;
                case 0x1: mnemonic = "sll"; break;
                case 0x2: mnemonic = "slt"; break;
                case 0x3: mnemonic = "sltu"; break;
                case 0x4: mnemonic = "xor"; break;
                case 0x5: mnemonic = (decoded->funct7 == 0x20) ? "sra" : "srl"; break;
                case 0x6: mnemonic = "or"; break;
                case 0x7: mnemonic = "and"; break;
            }
            sprintf(assembly_str, "%-7s x%d, x%d, x%d", mnemonic, decoded->rd, decoded->rs1, decoded->rs2);
            break;

        case OP_I_TYPE_ARITH:
            switch (decoded->funct3) {
                case 0x0: mnemonic = "addi"; break;
                case 0x1: mnemonic = "slli"; break;
                case 0x2: mnemonic = "slti"; break;
                case 0x3: mnemonic = "sltiu"; break;
                case 0x4: mnemonic = "xori"; break;
                case 0x5: mnemonic = (decoded->funct7 == 0x20) ? "srai" : "srli"; break;
                case 0x6: mnemonic = "ori"; break;
                case 0x7: mnemonic = "andi"; break;
            }
            if (decoded->funct3 == 0x1 || decoded->funct3 == 0x5) {
                sprintf(assembly_str, "%-7s x%d, x%d, %d", mnemonic, decoded->rd, decoded->rs1, decoded->imm & 0x1F);
            } else {
                sprintf(assembly_str, "%-7s x%d, x%d, %d", mnemonic, decoded->rd, decoded->rs1, decoded->imm);
            }
            break;

        case OP_I_TYPE_LOAD:
            switch (decoded->funct3) {
                case 0x0: mnemonic = "lb"; break;
                case 0x1: mnemonic = "lh"; break;
                case 0x2: mnemonic = "lw"; break;
                case 0x4: mnemonic = "lbu"; break;
                case 0x5: mnemonic = "lhu"; break;
            }
            sprintf(assembly_str, "%-7s x%d, %d(x%d)", mnemonic, decoded->rd, decoded->imm, decoded->rs1);
            break;

        case OP_S_TYPE:
            switch (decoded->funct3) {
                case 0x0: mnemonic = "sb"; break;
                case 0x1: mnemonic = "sh"; break;
                case 0x2: mnemonic = "sw"; break;
            }
            sprintf(assembly_str, "%-7s x%d, %d(x%d)", mnemonic, decoded->rs2, decoded->imm, decoded->rs1);
            break;

        case OP_B_TYPE:
            switch (decoded->funct3) {
                case 0x0: mnemonic = "beq"; break;
                case 0x1: mnemonic = "bne"; break;
                case 0x4: mnemonic = "blt"; break;
                case 0x5: mnemonic = "bge"; break;
                case 0x6: mnemonic = "bltu"; break;
                case 0x7: mnemonic = "bgeu"; break;
            }
            sprintf(assembly_str, "%-7s x%d, x%d, %d", mnemonic, decoded->rs1, decoded->rs2, decoded->imm);
            break;

        case OP_U_TYPE_LUI:
            sprintf(assembly_str, "%-7s x%d, 0x%X", "lui", decoded->rd, (decoded->imm >> 12));
            break;

        case OP_U_TYPE_AUIPC:
            sprintf(assembly_str, "%-7s x%d, 0x%X", "auipc", decoded->rd, (decoded->imm >> 12));
            break;

        case OP_J_TYPE_JAL:
            sprintf(assembly_str, "%-7s x%d, %d", "jal", decoded->rd, decoded->imm);
            break;

        case OP_I_TYPE_JALR:
            sprintf(assembly_str, "%-7s x%d, %d(x%d)", "jalr", decoded->rd, decoded->imm, decoded->rs1);
            break;

        default:
            sprintf(assembly_str, "UNKNOWN");
            break;
    }

    printf("0x%08x  %08X    %s\n", address, inst, assembly_str);
}
#include <stdio.h>
#include <assert.h>
#include "../include/decoder.h"

#define GRN "\e[0;32m"
#define RED "\e[0;31m"
#define RST "\e[0m"

void test_r_type_decode() {
    decodedinst_t decoded;
    decode_instruction(0x003100B3, &decoded);
    
    assert(decoded.opcode == OP_R_TYPE);
    assert(decoded.rd == 1);
    assert(decoded.rs1 == 2);
    assert(decoded.rs2 == 3);
    assert(decoded.funct3 == 0x0);
    assert(decoded.funct7 == 0x00);
    printf(GRN "PASS" RST ": test_r_type_decode (add)\n");
}

void test_i_type_negative_immediate() {
    decodedinst_t decoded;
    decode_instruction(0xFFF10113, &decoded);
    
    assert(decoded.opcode == OP_I_TYPE_ARITH);
    assert(decoded.rd == 2);
    assert(decoded.rs1 == 2);
    assert(decoded.funct3 == 0x0);
    assert(decoded.imm == -1); 
    printf(GRN "PASS" RST ": test_i_type_negative_immediate (addi)\n");
}

void test_b_type_scrambled_immediate() {
    decodedinst_t decoded;
    decode_instruction(0xFE209CE3, &decoded);
    
    assert(decoded.opcode == OP_B_TYPE);
    assert(decoded.rs1 == 1);
    assert(decoded.rs2 == 2);
    assert(decoded.funct3 == 0x1);
    assert(decoded.imm == -8);
    printf(GRN "PASS" RST ": test_b_type_scrambled_immediate (bne)\n");
}

void test_s_type_negative_offset() {
    decodedinst_t decoded;
    decode_instruction(0xFE321F23, &decoded);
    
    assert(decoded.opcode == OP_S_TYPE);
    assert(decoded.rs1 == 4);
    assert(decoded.rs2 == 3);
    assert(decoded.funct3 == 0x1);
    assert(decoded.imm == -2);
    printf(GRN "PASS" RST ": test_s_type_negative_offset (sh)\n");
}

void test_u_type_upper_immediate() {
    decodedinst_t decoded;
    decode_instruction(0x123452B7, &decoded);
    
    assert(decoded.opcode == OP_U_TYPE_LUI);
    assert(decoded.rd == 5);
    assert(decoded.imm == 0x12345000);
    printf(GRN "PASS" RST ": test_u_type_upper_immediate (lui)\n");
}

void test_j_type_scrambled_negative_jump() {
    decodedinst_t decoded;
    decode_instruction(0xFFDFF06F, &decoded);
    
    assert(decoded.opcode == OP_J_TYPE_JAL);
    assert(decoded.rd == 0);
    assert(decoded.imm == -4);
    printf(GRN "PASS" RST ": test_j_type_scrambled_negative_jump (jal)\n");
}

int main() {
    printf("Starting Decoder SQA Unit Tests...\n");
    printf("------------------------------------\n");
    
    test_r_type_decode();
    test_i_type_negative_immediate();
    test_b_type_scrambled_immediate();
    test_s_type_negative_offset();
    test_u_type_upper_immediate();
    test_j_type_scrambled_negative_jump();
    
    printf("------------------------------------\n");
    printf(GRN "ALL SQA TESTS PASSED SUCCESSFULLY.\n" RST);
    
    return 0;
}
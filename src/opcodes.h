#ifndef OPCODES_H
#define OPCODES_H

#include <stdint.h>

typedef enum opcode_type {
    OP_NOP = 0, 
    OP_TRANSFER_8,
    OP_TRANSFER_16,
    OP_ARITHM_8,
    OP_ARITHM_16,
    OP_JMP,
    OP_CALL,
    OP_RET,
    OP_ROT,
    OP_MISC,
} opcode_t;

#define EACH_OP_CODE(_) \
    _(NOP) _(LXI_B_D16) _(STAX_B) _(INX_B) _(INR_B) _(DCR_B) _(MVI_B_D8) _(RLC) \
    _(NOP_DUP_0) _(DAD_B) _(LDAX_B) _(DCX_B) _(INR_C) _(DCR_C) _(MVI_C_D8) _(RRC) \
    _(NOP_DUP_1) _(LXI_D_D16) _(STAX_D) _(INX_D) _(INR_D) _(DCR_D) _(MVI_D) _(RAL) \
    _(NOP_DUP_2) _(DAD_D) _(LDAX_D) _(DCX_D) _(INR_E) _(DCR_E) _(MVI_E_D8) _(RAR) \
    _(NOP_DUP_3) _(LXI_H_D16) _(SHLD_A16) _(INX_H) _(INR_H) _(DCR_H) _(MVI_H_D8) _(DAA) \
    _(NOP_DUP_4) _(DAD_H) _(LHLD_A16) _(DCX_H) _(INR_L) _(DCR_L) _(MVI_L_D8) _(CMA) \
    _(NOP_DUP_5) _(LXI_SP_D16) _(STA_A16) _(INX_SP) _(INR_M) _(DCR_M) _(MVI_M_D8) _(STC) \
    _(NOP_DUP_6) _(DAD_SP) _(LDA_A16) _(DCX_SP) _(INR_A) _(DCR_A) _(MVI_A_D8) _(CMC) \
    _(MOV_B_B) _(MOV_B_C) _(MOV_B_D) _(MOV_B_E) _(MOV_B_H) _(MOV_B_L) _(MOV_B_M) _(MOV_B_A) \
    _(MOV_C_B) _(MOV_C_C) _(MOV_C_D) _(MOV_C_E) _(MOV_C_H) _(MOV_C_L) _(MOV_C_M) _(MOV_C_A) \
    _(MOV_D_B) _(MOV_D_C) _(MOV_D_D) _(MOV_D_E) _(MOV_D_H) _(MOV_D_L) _(MOV_D_M) _(MOV_D_A) \
    _(MOV_E_B) _(MOV_E_C) _(MOV_E_D) _(MOV_E_E) _(MOV_E_H) _(MOV_E_L) _(MOV_E_M) _(MOV_E_A) \
    _(MOV_H_B) _(MOV_H_C) _(MOV_H_D) _(MOV_H_E) _(MOV_H_H) _(MOV_H_L) _(MOV_H_M) _(MOV_H_A) \
    _(MOV_L_B) _(MOV_L_C) _(MOV_L_D) _(MOV_L_E) _(MOV_L_H) _(MOV_L_L) _(MOV_L_M) _(MOV_L_A) \
    _(MOV_M_B) _(MOV_M_C) _(MOV_M_D) _(MOV_M_E) _(MOV_M_H) _(MOV_M_L) _(HLT) _(MOV_M_A) \
    _(MOV_A_B) _(MOV_A_C) _(MOV_A_D) _(MOV_A_E) _(MOV_A_H) _(MOV_A_L) _(MOV_A_M) _(MOV_A_A) \
    _(ADD_B) _(ADD_C) _(ADD_D) _(ADD_E) _(ADD_H) _(ADD_L) _(ADD_M) _(ADD_A) \
    _(ADC_B) _(ADC_C) _(ADC_D) _(ADC_E) _(ADC_H) _(ADC_L) _(ADC_M) _(ADC_A) \
    _(SUB_B) _(SUB_C) _(SUB_D) _(SUB_E) _(SUB_H) _(SUB_L) _(SUB_M) _(SUB_A) \
    _(SBB_B) _(SBB_C) _(SBB_D) _(SBB_E) _(SBB_H) _(SBB_L) _(SBB_M) _(SBB_A) \
    _(ANA_B) _(ANA_C) _(ANA_D) _(ANA_E) _(ANA_H) _(ANA_L) _(ANA_M) _(ANA_A) \
    _(XRA_B) _(XRA_C) _(XRA_D) _(XRA_E) _(XRA_H) _(XRA_L) _(XRA_M) _(XRA_A) \
    _(ORA_B) _(ORA_C) _(ORA_D) _(ORA_E) _(ORA_H) _(ORA_L) _(ORA_M) _(ORA_A) \
    _(CMP_B) _(CMP_C) _(CMP_D) _(CMP_E) _(CMP_H) _(CMP_L) _(CMP_M) _(CMP_A) \
    _(RNZ) _(POP_B) _(JNZ_A16) _(JMP_A16) _(CNZ_A16) _(PUSH_B) _(ADI_D8) _(RST_0) \
    _(RZ) _(RET) _(JZ_A16) _(JMP_A16_DUP_0) _(CZ) _(CALL_A16) _(ACI_D8) _(RST_1) \
    _(RNC) _(POP_D) _(JNC_A16) _(OUT_D8) _(CNC_A16) _(PUSH_D) _(SUI_D8) _(RST_2) \
    _(RC) _(RET_DUP_0) _(JC_A16) _(IN_D8) _(CC_A16) _(CALL_A16_DUP_0) _(SBI_D8) _(RST_3) \
    _(RPO) _(POP_H) _(JPO_A16) _(XTHL) _(CPO_A16) _(PUSH_H) _(ANI_D8) _(RST_4) \
    _(RPE) _(PCHL) _(JPE_A16) _(XCHG) _(CPE_A16) _(CALL_A16_DUP_1) _(XRI_D8) _(RST_5) \
    _(RP) _(POP_PSW) _(JP_A16) _(DI) _(CP_A16) _(PUSH_PSW) _(ORI_D8) _(RST_6) \
    _(RM) _(SPHL) _(JM_A16) _(EI) _(CM_A16) _(CALL_A16_DUP_2) _(CPI_D8) _(RST_7)


#define OP_CODES_CNT 256
#define AS_BARE_NAME_COMMA(TOKEN) TOKEN ,
#define AS_STRING_COMMA(TOKEN) #TOKEN ,

typedef enum opcode {
    EACH_OP_CODE(AS_BARE_NAME_COMMA)
} opcode;

const char* opcode_str[OP_CODES_CNT] {
    EACH_OP_CODE(AS_STRING_COMMA)
};

#undef AS_STRING_COMMA
#undef AS_BARE_NAME_COMMA


typedef struct {
    uint8_t  length;
    uint8_t  t_duration;
    uint8_t  n_duration;
    opcode_t op_type;
} op_code_detail;

op_code_detail op_code_details[OP_CODES_CNT] = {
    {1,  4, 0, OP_NOP},  // NOP
    {3, 10, 0, OP_MISC}, // LXI_B_D16
    {1,  7, 0, OP_MISC}, // STAX_B
    {1,  5, 0, OP_MISC}, // INX_B
    {1,  5, 0, OP_MISC}, // INR_B
    {1,  5, 0, OP_MISC}, // DCR_B
    {2,  7, 0, OP_MISC}, // MVI_B_D8
    {1,  4, 0, OP_MISC}, // RLC
    {1,  4, 0, OP_NOP},  // NOP_DUP_0
    {1, 10, 0, OP_MISC}, // DAD_B
    {1,  7, 0, OP_MISC}, // LDAX_B
    {1,  5, 0, OP_MISC}, // DCX_B
    {1,  5, 0, OP_MISC}, // INR_C
    {1,  5, 0, OP_MISC}, // DCR_C
    {2,  7, 0, OP_MISC}, // MVI_C_D8
    {1,  4, 0, OP_MISC}, // RRC
    {1,  4, 0, OP_NOP},  // NOP_DUP_1
    {3, 10, 0, OP_MISC}, // LXI_D_D16
    {1,  7, 0, OP_MISC}, // STAX_D
    {1,  5, 0, OP_MISC}, // INX_D
    {1,  5, 0, OP_MISC}, // INR_D
    {1,  5, 0, OP_MISC}, // DCR_D
    {2,  7, 0, OP_MISC}, // MVI_D
    {1,  4, 0, OP_MISC}, // RAL
    {1,  4, 0, OP_NOP},  // NOP_DUP_2
    {1, 10, 0, OP_MISC}, // DAD_D
    {1,  7, 0, OP_MISC}, // LDAX_D
    {1,  5, 0, OP_MISC}, // DCX_D
    {1,  5, 0, OP_MISC}, // INR_E
    {1,  5, 0, OP_MISC}, // DCR_E
    {2,  7, 0, OP_MISC}, // MVI_E_D8
    {1,  4, 0, OP_MISC}, // RAR
    {1,  4, 0, OP_NOP},  // NOP_DUP_3
    {3, 10, 0, OP_MISC}, // LXI_H_D16
    {3, 16, 0, OP_MISC}, // SHLD_A16
    {1,  5, 0, OP_MISC}, // INX_H
    {1,  5, 0, OP_MISC}, // INR_H
    {1,  5, 0, OP_MISC}, // DCR_H
    {2,  7, 0, OP_MISC}, // MVI_H_D8
    {1,  4, 0, OP_MISC}, // DAA
    {1,  4, 0, OP_NOP},  // NOP_DUP_4
    {1, 10, 0, OP_MISC}, // DAD_H
    {3, 16, 0, OP_MISC}, // LHLD_A16
    {1,  5, 0, OP_MISC}, // DCX_H
    {1,  5, 0, OP_MISC}, // INR_L
    {1,  5, 0, OP_MISC}, // DCR_L
    {2,  7, 0, OP_MISC}, // MVI_L_D8
    {1,  4, 0, OP_MISC}, // CMA
    {1,  4, 0, OP_NOP},  // NOP_DUP_5
    {3, 10, 0, OP_MISC}, // LXI_SP_D16
    {3, 13, 0, OP_MISC}, // STA_A16
    {1,  5, 0, OP_MISC}, // INX_SP
    {1, 10, 0, OP_MISC}, // INR_M
    {1, 10, 0, OP_MISC}, // DCR_M
    {2, 10, 0, OP_MISC}, // MVI_M_D8
    {1,  4, 0, OP_MISC}, // STC
    {1,  4, 0, OP_NOP},  // NOP_DUP_6
    {1, 10, 0, OP_MISC}, // DAD_SP
    {3, 13, 0, OP_MISC}, // LDA_A16
    {1,  5, 0, OP_MISC}, // DCX_SP
    {1,  5, 0, OP_MISC}, // INR_A
    {1,  5, 0, OP_MISC}, // DCR_A
    {2,  7, 0, OP_MISC}, // MVI_A_D8
    {1,  4, 0, OP_MISC}, // CMC
    {1,  5, 0, OP_MISC}, // MOV_B_B
    {1,  5, 0, OP_MISC}, // MOV_B_C
    {1,  5, 0, OP_MISC}, // MOV_B_D
    {1,  5, 0, OP_MISC}, // MOV_B_E
    {1,  5, 0, OP_MISC}, // MOV_B_H
    {1,  5, 0, OP_MISC}, // MOV_B_L
    {1,  7, 0, OP_MISC}, // MOV_B_M
    {1,  5, 0, OP_MISC}, // MOV_B_A
    {1,  5, 0, OP_MISC}, // MOV_C_B
    {1,  5, 0, OP_MISC}, // MOV_C_C
    {1,  5, 0, OP_MISC}, // MOV_C_D
    {1,  5, 0, OP_MISC}, // MOV_C_E
    {1,  5, 0, OP_MISC}, // MOV_C_H
    {1,  5, 0, OP_MISC}, // MOV_C_L
    {1,  7, 0, OP_MISC}, // MOV_C_M
    {1,  5, 0, OP_MISC}, // MOV_C_A
    {1,  5, 0, OP_MISC}, // MOV_D_B
    {1,  5, 0, OP_MISC}, // MOV_D_C
    {1,  5, 0, OP_MISC}, // MOV_D_D
    {1,  5, 0, OP_MISC}, // MOV_D_E
    {1,  5, 0, OP_MISC}, // MOV_D_H
    {1,  5, 0, OP_MISC}, // MOV_D_L
    {1,  7, 0, OP_MISC}, // MOV_D_M
    {1,  5, 0, OP_MISC}, // MOV_D_A
    {1,  5, 0, OP_MISC}, // MOV_E_B
    {1,  5, 0, OP_MISC}, // MOV_E_C
    {1,  5, 0, OP_MISC}, // MOV_E_D
    {1,  5, 0, OP_MISC}, // MOV_E_E
    {1,  5, 0, OP_MISC}, // MOV_E_H
    {1,  5, 0, OP_MISC}, // MOV_E_L
    {1,  7, 0, OP_MISC}, // MOV_E_M
    {1,  5, 0, OP_MISC}, // MOV_E_A
    {1,  5, 0, OP_MISC}, // MOV_H_B
    {1,  5, 0, OP_MISC}, // MOV_H_C
    {1,  5, 0, OP_MISC}, // MOV_H_D
    {1,  5, 0, OP_MISC}, // MOV_H_E
    {1,  5, 0, OP_MISC}, // MOV_H_H
    {1,  5, 0, OP_MISC}, // MOV_H_L
    {1,  7, 0, OP_MISC}, // MOV_H_M
    {1,  5, 0, OP_MISC}, // MOV_H_A
    {1,  5, 0, OP_MISC}, // MOV_L_B
    {1,  5, 0, OP_MISC}, // MOV_L_C
    {1,  5, 0, OP_MISC}, // MOV_L_D
    {1,  5, 0, OP_MISC}, // MOV_L_E
    {1,  5, 0, OP_MISC}, // MOV_L_H
    {1,  5, 0, OP_MISC}, // MOV_L_L
    {1,  7, 0, OP_MISC}, // MOV_L_M
    {1,  5, 0, OP_MISC}, // MOV_L_A
    {1,  7, 0, OP_MISC}, // MOV_M_B
    {1,  7, 0, OP_MISC}, // MOV_M_C
    {1,  7, 0, OP_MISC}, // MOV_M_D
    {1,  7, 0, OP_MISC}, // MOV_M_E
    {1,  7, 0, OP_MISC}, // MOV_M_H
    {1,  7, 0, OP_MISC}, // MOV_M_L
    {1,  7, 0, OP_MISC}, // HLT
    {1,  7, 0, OP_MISC}, // MOV_M_A
    {1,  5, 0, OP_MISC}, // MOV_A_B
    {1,  5, 0, OP_MISC}, // MOV_A_C
    {1,  5, 0, OP_MISC}, // MOV_A_D
    {1,  5, 0, OP_MISC}, // MOV_A_E
    {1,  5, 0, OP_MISC}, // MOV_A_H
    {1,  5, 0, OP_MISC}, // MOV_A_L
    {1,  7, 0, OP_MISC}, // MOV_A_M
    {1,  5, 0, OP_MISC}, // MOV_A_A
    {1,  4, 0, OP_MISC}, // ADD_B
    {1,  4, 0, OP_MISC}, // ADD_C
    {1,  4, 0, OP_MISC}, // ADD_D
    {1,  4, 0, OP_MISC}, // ADD_E
    {1,  4, 0, OP_MISC}, // ADD_H
    {1,  4, 0, OP_MISC}, // ADD_L
    {1,  7, 0, OP_MISC}, // ADD_M
    {1,  4, 0, OP_MISC}, // ADD_A
    {1,  4, 0, OP_MISC}, // ADC_B
    {1,  4, 0, OP_MISC}, // ADC_C
    {1,  4, 0, OP_MISC}, // ADC_D
    {1,  4, 0, OP_MISC}, // ADC_E
    {1,  4, 0, OP_MISC}, // ADC_H
    {1,  4, 0, OP_MISC}, // ADC_L
    {1,  7, 0, OP_MISC}, // ADC_M
    {1,  4, 0, OP_MISC}, // ADC_A
    {1,  4, 0, OP_MISC}, // SUB_B
    {1,  4, 0, OP_MISC}, // SUB_C
    {1,  4, 0, OP_MISC}, // SUB_D
    {1,  4, 0, OP_MISC}, // SUB_E
    {1,  4, 0, OP_MISC}, // SUB_H
    {1,  4, 0, OP_MISC}, // SUB_L
    {1,  7, 0, OP_MISC}, // SUB_M
    {1,  4, 0, OP_MISC}, // SUB_A
    {1,  4, 0, OP_MISC}, // SBB_B
    {1,  4, 0, OP_MISC}, // SBB_C
    {1,  4, 0, OP_MISC}, // SBB_D
    {1,  4, 0, OP_MISC}, // SBB_E
    {1,  4, 0, OP_MISC}, // SBB_H
    {1,  4, 0, OP_MISC}, // SBB_L
    {1,  7, 0, OP_MISC}, // SBB_M
    {1,  4, 0, OP_MISC}, // SBB_A
    {1,  4, 0, OP_MISC}, // ANA_B
    {1,  4, 0, OP_MISC}, // ANA_C
    {1,  4, 0, OP_MISC}, // ANA_D
    {1,  4, 0, OP_MISC}, // ANA_E
    {1,  4, 0, OP_MISC}, // ANA_H
    {1,  4, 0, OP_MISC}, // ANA_L
    {1,  7, 0, OP_MISC}, // ANA_M
    {1,  4, 0, OP_MISC}, // ANA_A
    {1,  4, 0, OP_MISC}, // XRA_B
    {1,  4, 0, OP_MISC}, // XRA_C
    {1,  4, 0, OP_MISC}, // XRA_D
    {1,  4, 0, OP_MISC}, // XRA_E
    {1,  4, 0, OP_MISC}, // XRA_H
    {1,  4, 0, OP_MISC}, // XRA_L
    {1,  7, 0, OP_MISC}, // XRA_M
    {1,  4, 0, OP_MISC}, // XRA_A
    {1,  4, 0, OP_MISC}, // ORA_B
    {1,  4, 0, OP_MISC}, // ORA_C
    {1,  4, 0, OP_MISC}, // ORA_D
    {1,  4, 0, OP_MISC}, // ORA_E
    {1,  4, 0, OP_MISC}, // ORA_H
    {1,  4, 0, OP_MISC}, // ORA_L
    {1,  7, 0, OP_MISC}, // ORA_M
    {1,  4, 0, OP_MISC}, // ORA_A
    {1,  4, 0, OP_MISC}, // CMP_B
    {1,  4, 0, OP_MISC}, // CMP_C
    {1,  4, 0, OP_MISC}, // CMP_D
    {1,  4, 0, OP_MISC}, // CMP_E
    {1,  4, 0, OP_MISC}, // CMP_H
    {1,  4, 0, OP_MISC}, // CMP_L
    {1,  7, 0, OP_MISC}, // CMP_M
    {1,  4, 0, OP_MISC}, // CMP_A
    {1,  11,5, OP_MISC}, // RNZ
    {1, 10, 0, OP_MISC}, // POP_B
    {3, 10, 0, OP_MISC}, // JNZ_A16
    {3, 10, 0, OP_MISC}, // JMP_A16
    {3, 17,11, OP_MISC}, // CNZ_A16
    {1, 11, 0, OP_MISC}, // PUSH_B
    {2,  7, 0, OP_MISC}, // ADI_D8
    {1, 11, 0, OP_MISC}, // RST_0
    {1, 11, 5, OP_MISC}, // RZ
    {1, 10, 0, OP_MISC}, // RET
    {3, 10, 0, OP_MISC}, // JZ_A16
    {3, 10, 0, OP_MISC}, // JMP_A16_DUP_0
    {3, 17,11, OP_MISC}, // CZ
    {3, 17, 0, OP_MISC}, // CALL_A16
    {2,  7, 0, OP_MISC}, // ACI_D8
    {1, 11, 0, OP_MISC}, // RST_1
    {1, 11, 5, OP_MISC}, // RNC
    {1, 10, 0, OP_MISC}, // POP_D
    {3, 10, 0, OP_MISC}, // JNC_A16
    {2, 10, 0, OP_MISC}, // OUT_D8
    {3, 17,11, OP_MISC}, // CNC_A16
    {1, 11, 0, OP_MISC}, // PUSH_D
    {2,  7, 0, OP_MISC}, // SUI_D8
    {1, 11, 0, OP_MISC}, // RST_2
    {1,  11,5, OP_MISC}, // RC
    {1, 10, 0, OP_MISC}, // RET_DUP_0
    {3, 10, 0, OP_MISC}, // JC_A16
    {2, 10, 0, OP_MISC}, // IN_D8
    {3, 17,11, OP_MISC}, // CC_A16
    {3, 17, 0, OP_MISC}, // CALL_A16_DUP_0
    {2,  7, 0, OP_MISC}, // SBI_D8
    {1, 11, 0, OP_MISC}, // RST_3
    {1, 11, 5, OP_MISC}, // RPO
    {1, 10, 0, OP_MISC}, // POP_H
    {3, 10, 0, OP_MISC}, // JPO_A16
    {1, 18, 0, OP_MISC}, // XTHL
    {3, 17,11, OP_MISC}, // CPO_A16
    {1, 11, 0, OP_MISC}, // PUSH_H
    {2,  7, 0, OP_MISC}, // ANI_D8
    {1, 11, 0, OP_MISC}, // RST_4
    {1, 11, 5, OP_MISC}, // RPE
    {1,  5, 0, OP_MISC}, // PCHL
    {3, 10, 0, OP_MISC}, // JPE_A16
    {1,  5, 0, OP_MISC}, // XCHG
    {3, 17,11, OP_MISC}, // CPE_A16
    {3, 17, 0, OP_MISC}, // CALL_A16_DUP_1
    {2,  7, 0, OP_MISC}, // XRI_D8
    {1, 11, 0, OP_MISC}, // RST_5
    {1, 11, 5, OP_MISC}, // RP
    {1, 10, 0, OP_MISC}, // POP_PSW
    {3, 10, 0, OP_MISC}, // JP_A16
    {1,  4, 0, OP_MISC}, // DI
    {3, 17,11, OP_MISC}, // CP_A16
    {1, 11, 0, OP_MISC}, // PUSH_PSW
    {2,  7, 0, OP_MISC}, // ORI_D8
    {1, 11, 0, OP_MISC}, // RST_6
    {1,  11,5, OP_MISC}, // RM
    {1,  5, 0, OP_MISC}, // SPHL
    {3, 10, 0, OP_MISC}, // JM_A16
    {1,  4, 0, OP_MISC}, // EI
    {3, 17,11, OP_MISC}, // CM_A16
    {3, 17, 0, OP_MISC}, // CALL_A16_DUP_2
    {2,  7, 0, OP_MISC}, // CPI_D8
    {1, 11, 0, OP_MISC}, // RST_7
};

#endif

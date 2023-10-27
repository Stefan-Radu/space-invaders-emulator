#ifndef CPU_8080_H
#define CPU_8080_H

#include <stdint.h>

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
    void (*op_code_action)(void);
} op_code_detail;

extern op_code_detail op_code_details[OP_CODES_CNT];

void print_registers(void);

#endif

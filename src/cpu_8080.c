#include <stdint.h>
#include <stdio.h>

#include "cpu_8080.h"

uint8_t  memory[0x10000];

uint16_t stack_pointer;
uint16_t program_counter;

typedef uint8_t  reg_t;
typedef uint16_t w_reg_t;

// assuming little endian
typedef union {
    struct { 
        union { 
        struct {
            // carry flag: last addition carry of last subtraction borrow
            uint8_t cf:    1; 
            // not used; default -1
            uint8_t nu_10: 1; 
            // parity: set if the number of -1 bits in the result is even
            uint8_t pa:    1; 
            // not used: default 0
            uint8_t nu_00: 1; 
            // used for binary coded decimal arithmetic?? TODO
            uint8_t ac:    1; 
            // not used: default 0
            uint8_t nu_01: 1; 
            // set if the result is zero
            uint8_t zf:    1; 
            // set if the result is negative
            uint8_t sf:    1;
        }; // independent flags as bits
        reg_t F; }; reg_t A;
        reg_t C;    reg_t B; 
        reg_t E;    reg_t D; 
        reg_t L;    reg_t H; 
    };
    struct {
        w_reg_t PSW; // (A|F) (PSW - program status word)
        w_reg_t BC;  // (B|C) (reffered to as B)
        w_reg_t DE;  // (D|E) (reffered to as D)
        w_reg_t HL;  // (H|L) (reffered to as H)
    };
} registers;

static registers regs;

// ============ INSTRUCTIONS ============

// TODO lazy flag evaluation

/* return the byte_offset'th byte from the current instruction */
inline int8_t get_op_byte(int8_t byte_offset) {
    // TODO check if this is correct at some point;
    return memory[program_counter + byte_offset];
}

/* return the byte_cnt'th byte from the current instruction */
inline int16_t get_op_2bytes(int8_t byte_offset) {
    // TODO check if this is correct at some point;
    int16_t ret = memory[program_counter + byte_offset];
    return (ret << 8) | memory[program_counter + byte_offset + 1];
}

/* nop */
inline void nop(void) {}

/* ========================================================== */
/* =================== 8-bit transfers ====================== */
/* ========================================================== */

/* move value from register r2 to register r1 */
static inline void mov_reg_reg(reg_t *r1, const reg_t r2) {
    *r1 = r2;
}

/* move value from memory at address pointed by HL to register r */
static inline void mov_reg_mem(reg_t *r) {
    *r = memory[regs.HL];
}

/* move value from register r to memory at address pointed by HL */
static inline void mov_mem_reg(const reg_t r) {
    memory[regs.HL] = r;
}

/* target: B */
static inline void mov_b_b(void) { mov_reg_reg(&regs.B, regs.B); }
static inline void mov_b_c(void) { mov_reg_reg(&regs.B, regs.C); }
static inline void mov_b_d(void) { mov_reg_reg(&regs.B, regs.D); }
static inline void mov_b_e(void) { mov_reg_reg(&regs.B, regs.E); }
static inline void mov_b_h(void) { mov_reg_reg(&regs.B, regs.H); }
static inline void mov_b_l(void) { mov_reg_reg(&regs.B, regs.L); }
static inline void mov_b_m(void) { mov_reg_mem(&regs.B); }
static inline void mov_b_a(void) { mov_reg_reg(&regs.B, regs.A); }

/* target: C */
static inline void mov_c_b(void) { mov_reg_reg(&regs.C, regs.B); }
static inline void mov_c_c(void) { mov_reg_reg(&regs.C, regs.C); }
static inline void mov_c_d(void) { mov_reg_reg(&regs.C, regs.D); }
static inline void mov_c_e(void) { mov_reg_reg(&regs.C, regs.E); }
static inline void mov_c_h(void) { mov_reg_reg(&regs.C, regs.H); }
static inline void mov_c_l(void) { mov_reg_reg(&regs.C, regs.L); }
static inline void mov_c_m(void) { mov_reg_mem(&regs.C); }
static inline void mov_c_a(void) { mov_reg_reg(&regs.C, regs.A); }

/* target: D */
static inline void mov_d_b(void) { mov_reg_reg(&regs.D, regs.B); }
static inline void mov_d_c(void) { mov_reg_reg(&regs.D, regs.C); }
static inline void mov_d_d(void) { mov_reg_reg(&regs.D, regs.D); }
static inline void mov_d_e(void) { mov_reg_reg(&regs.D, regs.E); }
static inline void mov_d_h(void) { mov_reg_reg(&regs.D, regs.H); }
static inline void mov_d_l(void) { mov_reg_reg(&regs.D, regs.L); }
static inline void mov_d_m(void) { mov_reg_mem(&regs.D); }
static inline void mov_d_a(void) { mov_reg_reg(&regs.D, regs.A); }

/* target: E */
static inline void mov_e_b(void) { mov_reg_reg(&regs.E, regs.B); }
static inline void mov_e_c(void) { mov_reg_reg(&regs.E, regs.C); }
static inline void mov_e_d(void) { mov_reg_reg(&regs.E, regs.D); }
static inline void mov_e_e(void) { mov_reg_reg(&regs.E, regs.E); }
static inline void mov_e_h(void) { mov_reg_reg(&regs.E, regs.H); }
static inline void mov_e_l(void) { mov_reg_reg(&regs.E, regs.L); }
static inline void mov_e_m(void) { mov_reg_mem(&regs.E); }
static inline void mov_e_a(void) { mov_reg_reg(&regs.E, regs.A); }

/* target: F */
static inline void mov_h_b(void) { mov_reg_reg(&regs.H, regs.B); }
static inline void mov_h_c(void) { mov_reg_reg(&regs.H, regs.C); }
static inline void mov_h_d(void) { mov_reg_reg(&regs.H, regs.D); }
static inline void mov_h_e(void) { mov_reg_reg(&regs.H, regs.E); }
static inline void mov_h_h(void) { mov_reg_reg(&regs.H, regs.H); }
static inline void mov_h_l(void) { mov_reg_reg(&regs.H, regs.L); }
static inline void mov_h_m(void) { mov_reg_mem(&regs.H); }
static inline void mov_h_a(void) { mov_reg_reg(&regs.H, regs.A); }

/* target: L */
static inline void mov_l_b(void) { mov_reg_reg(&regs.L, regs.B); }
static inline void mov_l_c(void) { mov_reg_reg(&regs.L, regs.C); }
static inline void mov_l_d(void) { mov_reg_reg(&regs.L, regs.D); }
static inline void mov_l_e(void) { mov_reg_reg(&regs.L, regs.E); }
static inline void mov_l_h(void) { mov_reg_reg(&regs.L, regs.H); }
static inline void mov_l_l(void) { mov_reg_reg(&regs.L, regs.L); }
static inline void mov_l_m(void) { mov_reg_mem(&regs.L); }
static inline void mov_l_a(void) { mov_reg_reg(&regs.L, regs.A); }

/* target: memory */
static inline void mov_m_b(void) { mov_mem_reg(regs.B); }
static inline void mov_m_c(void) { mov_mem_reg(regs.C); }
static inline void mov_m_d(void) { mov_mem_reg(regs.D); }
static inline void mov_m_e(void) { mov_mem_reg(regs.E); }
static inline void mov_m_h(void) { mov_mem_reg(regs.H); }
static inline void mov_m_l(void) { mov_mem_reg(regs.L); }
/* void hlt in category misc */
static inline void mov_m_a(void) { mov_mem_reg(regs.A); }

/* target: A */
static inline void mov_a_b(void) { mov_reg_reg(&regs.A, regs.B); }
static inline void mov_a_c(void) { mov_reg_reg(&regs.A, regs.C); }
static inline void mov_a_d(void) { mov_reg_reg(&regs.A, regs.D); }
static inline void mov_a_e(void) { mov_reg_reg(&regs.A, regs.E); }
static inline void mov_a_h(void) { mov_reg_reg(&regs.A, regs.H); }
static inline void mov_a_l(void) { mov_reg_reg(&regs.A, regs.L); }
static inline void mov_a_m(void) { mov_reg_mem(&regs.A); }
static inline void mov_a_a(void) { mov_reg_reg(&regs.A, regs.A); }

/* ======================== mvi ============================= */

/* mov imediate value (2nd byte of opcode) to register r */
static inline void mvi_reg(reg_t *r, reg_t val) {
    *r = val;
}

static inline void mvi_b(void) { 
    uint8_t val = get_op_byte(1);
    mvi_reg(&regs.B, val);
}

static inline void mvi_c(void) { 
    uint8_t val = get_op_byte(1);
    mvi_reg(&regs.C, val);
}

static inline void mvi_d(void) { 
    uint8_t val = get_op_byte(1);
    mvi_reg(&regs.D, val);
}

static inline void mvi_e(void) { 
    uint8_t val = get_op_byte(1);
    mvi_reg(&regs.E, val);
}

static inline void mvi_h(void) { 
    uint8_t val = get_op_byte(1);
    mvi_reg(&regs.H, val);
}

static inline void mvi_l(void) { 
    uint8_t val = get_op_byte(1);
    mvi_reg(&regs.L, val);
}

/* mov imediate value (2nd byte of opcode) to memory address HL */
static inline void mvi_m(void) { 
    uint8_t val = get_op_byte(1);
    memory[regs.HL] = val;
}

static inline void mvi_a(void) { 
    uint8_t val = get_op_byte(1);
    mvi_reg(&regs.A, val);
}

/* ======================== lda ============================= */

/* load value at address into register A */

static inline void ldax_b() {
    regs.A = memory[regs.BC];
}

static inline void ldax_d() {
    regs.A = memory[regs.DE];
}

static inline void lda() {
    regs.A = memory[get_op_2bytes(1)];
}

/* ======================== sta ============================= */

/* store value from register A at address */

static inline void stax_b() {
    memory[regs.BC] = regs.A;
}

static inline void stax_d() {
    memory[regs.DE] = regs.A;
}

static inline void sta() {
    memory[get_op_2bytes(1)] = regs.A;
}

/* ========================================================== */
/* =================== 16-bit transfers ===================== */
/* ========================================================== */


/*// 8-bit arithmetic*/
/*#define INR(reg) registers.(reg) += 1\*/
                /*if (registers.(reg) == 0) registers.zf = 1; \*/
                /*if (registers.(reg) == 0) registers.cf = 1; \*/
                /*if (registers.(reg) & (1 << 7)) registers.sf = 1; \*/
                /*if (*/
            


/*// 16-bit arithmetic*/
/*#define INX(reg) (registers.(reg) += 1)*/

op_code_detail op_code_details[OP_CODES_CNT] = {
    {1,  4, 0, &nop},  // NOP
    {3, 10, 0, OP_MISC}, // LXI_B_D16
    {1,  7, 0, &stax_b}, // STAX_B
    {1,  5, 0, OP_MISC}, // INX_B
    {1,  5, 0, OP_MISC}, // INR_B
    {1,  5, 0, OP_MISC}, // DCR_B
    {2,  7, 0, &mvi_b}, // MVI_B_D8
    {1,  4, 0, OP_MISC}, // RLC
    {1,  4, 0, &nop},  // NOP_DUP_0
    {1, 10, 0, OP_MISC}, // DAD_B
    {1,  7, 0, &ldax_b}, // LDAX_B
    {1,  5, 0, OP_MISC}, // DCX_B
    {1,  5, 0, OP_MISC}, // INR_C
    {1,  5, 0, OP_MISC}, // DCR_C
    {2,  7, 0, &mvi_c}, // MVI_C_D8
    {1,  4, 0, OP_MISC}, // RRC
    {1,  4, 0, &nop},  // NOP_DUP_1
    {3, 10, 0, OP_MISC}, // LXI_D_D16
    {1,  7, 0, &stax_d}, // STAX_D
    {1,  5, 0, OP_MISC}, // INX_D
    {1,  5, 0, OP_MISC}, // INR_D
    {1,  5, 0, OP_MISC}, // DCR_D
    {2,  7, 0, &mvi_d}, // MVI_D
    {1,  4, 0, OP_MISC}, // RAL
    {1,  4, 0, &nop},  // NOP_DUP_2
    {1, 10, 0, OP_MISC}, // DAD_D
    {1,  7, 0, &ldax_d}, // LDAX_D
    {1,  5, 0, OP_MISC}, // DCX_D
    {1,  5, 0, OP_MISC}, // INR_E
    {1,  5, 0, OP_MISC}, // DCR_E
    {2,  7, 0, &mvi_e}, // MVI_E_D8
    {1,  4, 0, OP_MISC}, // RAR
    {1,  4, 0, &nop},  // NOP_DUP_3
    {3, 10, 0, OP_MISC}, // LXI_H_D16
    {3, 16, 0, OP_MISC}, // SHLD_A16
    {1,  5, 0, OP_MISC}, // INX_H
    {1,  5, 0, OP_MISC}, // INR_H
    {1,  5, 0, OP_MISC}, // DCR_H
    {2,  7, 0, &mvi_h}, // MVI_H_D8
    {1,  4, 0, OP_MISC}, // DAA
    {1,  4, 0, &nop},  // NOP_DUP_4
    {1, 10, 0, OP_MISC}, // DAD_H
    {3, 16, 0, OP_MISC}, // LHLD_A16
    {1,  5, 0, OP_MISC}, // DCX_H
    {1,  5, 0, OP_MISC}, // INR_L
    {1,  5, 0, OP_MISC}, // DCR_L
    {2,  7, 0, &mvi_l}, // MVI_L_D8
    {1,  4, 0, OP_MISC}, // CMA
    {1,  4, 0, &nop},  // NOP_DUP_5
    {3, 10, 0, OP_MISC}, // LXI_SP_D16
    {3, 13, 0, &sta}, // STA_A16
    {1,  5, 0, OP_MISC}, // INX_SP
    {1, 10, 0, OP_MISC}, // INR_M
    {1, 10, 0, OP_MISC}, // DCR_M
    {2, 10, 0, &mvi_m}, // MVI_M_D8
    {1,  4, 0, OP_MISC}, // STC
    {1,  4, 0, &nop},  // NOP_DUP_6
    {1, 10, 0, OP_MISC}, // DAD_SP
    {3, 13, 0, &lda}, // LDA_A16
    {1,  5, 0, OP_MISC}, // DCX_SP
    {1,  5, 0, OP_MISC}, // INR_A
    {1,  5, 0, OP_MISC}, // DCR_A
    {2,  7, 0, &mvi_a}, // MVI_A_D8
    {1,  4, 0, OP_MISC}, // CMC
    {1,  5, 0, &mov_b_b}, // MOV_B_B
    {1,  5, 0, &mov_b_c}, // MOV_B_C
    {1,  5, 0, &mov_b_d}, // MOV_B_D
    {1,  5, 0, &mov_b_e}, // MOV_B_E
    {1,  5, 0, &mov_b_h}, // MOV_B_H
    {1,  5, 0, &mov_b_l}, // MOV_B_L
    {1,  7, 0, &mov_b_m}, // MOV_B_M
    {1,  5, 0, &mov_b_a}, // MOV_B_A
    {1,  5, 0, &mov_c_b}, // MOV_C_B
    {1,  5, 0, &mov_c_c}, // MOV_C_C
    {1,  5, 0, &mov_c_d}, // MOV_C_D
    {1,  5, 0, &mov_c_e}, // MOV_C_E
    {1,  5, 0, &mov_c_h}, // MOV_C_H
    {1,  5, 0, &mov_c_l}, // MOV_C_L
    {1,  7, 0, &mov_c_m}, // MOV_C_M
    {1,  5, 0, &mov_c_a}, // MOV_C_A
    {1,  5, 0, &mov_d_b}, // MOV_D_B
    {1,  5, 0, &mov_d_c}, // MOV_D_C
    {1,  5, 0, &mov_d_d}, // MOV_D_D
    {1,  5, 0, &mov_d_e}, // MOV_D_E
    {1,  5, 0, &mov_d_h}, // MOV_D_H
    {1,  5, 0, &mov_d_l}, // MOV_D_L
    {1,  7, 0, &mov_d_m}, // MOV_D_M
    {1,  5, 0, &mov_d_a}, // MOV_D_A
    {1,  5, 0, &mov_e_b}, // MOV_E_B
    {1,  5, 0, &mov_e_c}, // MOV_E_C
    {1,  5, 0, &mov_e_d}, // MOV_E_D
    {1,  5, 0, &mov_e_e}, // MOV_E_E
    {1,  5, 0, &mov_e_h}, // MOV_E_H
    {1,  5, 0, &mov_e_l}, // MOV_E_L
    {1,  7, 0, &mov_e_m}, // MOV_E_M
    {1,  5, 0, &mov_e_a}, // MOV_E_A
    {1,  5, 0, &mov_h_b}, // MOV_H_B
    {1,  5, 0, &mov_h_c}, // MOV_H_C
    {1,  5, 0, &mov_h_d}, // MOV_H_D
    {1,  5, 0, &mov_h_e}, // MOV_H_E
    {1,  5, 0, &mov_h_h}, // MOV_H_H
    {1,  5, 0, &mov_h_l}, // MOV_H_L
    {1,  7, 0, &mov_h_m}, // MOV_H_M
    {1,  5, 0, &mov_h_a}, // MOV_H_A
    {1,  5, 0, &mov_l_b}, // MOV_L_B
    {1,  5, 0, &mov_l_c}, // MOV_L_C
    {1,  5, 0, &mov_l_d}, // MOV_L_D
    {1,  5, 0, &mov_l_e}, // MOV_L_E
    {1,  5, 0, &mov_l_h}, // MOV_L_H
    {1,  5, 0, &mov_l_l}, // MOV_L_L
    {1,  7, 0, &mov_l_m}, // MOV_L_M
    {1,  5, 0, &mov_l_a}, // MOV_L_A
    {1,  7, 0, &mov_m_b}, // MOV_M_B
    {1,  7, 0, &mov_m_c}, // MOV_M_C
    {1,  7, 0, &mov_m_d}, // MOV_M_D
    {1,  7, 0, &mov_m_e}, // MOV_M_E
    {1,  7, 0, &mov_m_h}, // MOV_M_H
    {1,  7, 0, &mov_m_l}, // MOV_M_L
    {1,  7, 0, OP_MISC}, // HLT
    {1,  7, 0, &mov_m_a}, // MOV_M_A
    {1,  5, 0, &mov_a_b}, // MOV_A_B
    {1,  5, 0, &mov_a_c}, // MOV_A_C
    {1,  5, 0, &mov_a_d}, // MOV_A_D
    {1,  5, 0, &mov_a_e}, // MOV_A_E
    {1,  5, 0, &mov_a_h}, // MOV_A_H
    {1,  5, 0, &mov_a_l}, // MOV_A_L
    {1,  7, 0, &mov_a_m}, // MOV_A_M
    {1,  5, 0, &mov_a_a}, // MOV_A_A
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

static void test_set_registers() {
}

void print_registers() {
    test_set_registers();

    printf("AF: %hhx | %hhx\n", regs.A, regs.F);
    printf("BC: %hhx | %hhx\n", regs.B, regs.C);
    printf("DE: %hhx | %hhx\n", regs.D, regs.E);
    printf("HL: %hhx | %hhx\n", regs.H, regs.L);

    printf("----------------------------\n");

    printf("PSW: %hx\n", regs.PSW);
    printf("B: %hx\n", regs.BC);
    printf("D: %hx\n", regs.DE);
    printf("H: %hx\n", regs.HL);
}

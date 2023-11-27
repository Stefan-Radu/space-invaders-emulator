#include <stdint.h>
#include <stdio.h>

#include "cpu_8080.h"
#include "utils.h"

static uint8_t  memory[0x10000];

static uint16_t stack_pointer;
static uint16_t program_counter;

static uint16_t last_acc;
static uint16_t last_operand;
static int8_t flags_updated = 0; // TODO used this?

/* TODO last operation??? */
// TODO lazy flag evaluation

/* TODO figure out a way to compute flags out of last operation */

/* TODO write tests for all operations - decide on testing framework (simple rather than complicated) */

/* TODO implement lazy flag computation for all */
typedef enum {
    ADD8,
    SUB8, /* TODO notice that subtraction is just complement of two addition aka x + ~y + 1 */
    INR,
    CMP8,
    AND8,
    OR8,
    XOR8,
    ADC,
    SBB,
    DAD16,
} operation;

operation last_operation;

// assuming little endian
typedef union {
    struct { 
        union { 
        struct {
            /* TODO comments seem destroyed here */
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
        uint8_t F; }; uint8_t A;
        uint8_t C;    uint8_t B; 
        uint8_t E;    uint8_t D; 
        uint8_t L;    uint8_t H; 
    };
    struct {
        uint16_t PSW; // (A|F) (PSW - program status word)
        uint16_t BC;  // (B|C) (reffered to as B)
        uint16_t DE;  // (D|E) (reffered to as D)
        uint16_t HL;  // (H|L) (reffered to as H)
    };
} registers;

static registers regs;

/* ============ INSTRUCTIONS ============

 * references:
 * - Intel 8080 Programmer's Manual
 * - https://gist.github.com/joefg/634fa4a1046516d785c9
 * - https://www.pastraiser.com/cpu/i8080/i8080_opcodes.html */


/* get the byte_offset'th byte from the current instruction */
inline int8_t get_op_byte(int8_t byte_offset) {
    // TODO check if this is correct at some point;
    // TODO check if this needs offset
    // TODO if it needs offset, maybe have two different so I can avoid argument
    return memory[program_counter + byte_offset];
}

/* get the address from the current instruction */
inline int16_t get_op_address() {
    // TODO check if this is correct at some point;
    int16_t ret = memory[program_counter + 1];
    return (ret << 8) | memory[program_counter];
}

/* nop */
inline void nop(void) {}

/* ========================================================== */
/* =================== 8-bit transfers ====================== */
/* ========================================================== */

/* move value from register r2 to register r1 */
static inline void mov_reg_reg(uint8_t *r1, const uint8_t r2) {
    *r1 = r2;
}

/* move value from memory at address pointed by HL to register r */
static inline void mov_reg_mem(uint8_t *r) {
    *r = memory[regs.HL];
}

/* move value from register r to memory at address pointed by HL */
static inline void mov_mem_reg(const uint8_t r) {
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
static inline void mvi_reg(uint8_t *r, uint8_t val) {
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

static inline void ldax_b(void) {
    regs.A = memory[regs.BC];
}

static inline void ldax_d(void) {
    regs.A = memory[regs.DE];
}

static inline void lda(void) {
    regs.A = memory[get_op_address()];
}

/* ======================== sta ============================= */

/* store value from register A at address */

static inline void stax_b(void) {
    memory[regs.BC] = regs.A;
}

static inline void stax_d(void) {
    memory[regs.DE] = regs.A;
}

static inline void sta(void) {
    memory[get_op_address()] = regs.A;
}

/* ========================================================== */
/* =================== 16-bit transfers ===================== */
/* ========================================================== */

/* store in LH 16 bits from the current op */
static inline void lhld(void) {
    int16_t addr = get_op_address();
    regs.L = memory[addr];
    regs.H = memory[addr + 1];
}

/* store LH as 16 bits data at address */
static inline void shld(void) {
    int16_t addr = get_op_address();
    memory[addr]     = regs.L;
    memory[addr + 1] = regs.H;
}

/* Load 16 bits from the current op big regsiters / SP */

static inline void lxi_b(void) {
    // TODO to check byte order
    regs.BC = get_op_address();
}

static inline void lxi_d(void) {
    // TODO to check byte order
    regs.DE = get_op_address();
}

static inline void lxi_h(void) {
    // TODO to check byte order
    regs.HL = get_op_address();
}

static inline void lxi_sp(void) {
    // TODO to check byte order
    stack_pointer = (get_op_byte(2) << 8)
        | get_op_byte(1);
}

/* push big register onto the stack; reverse of pop */

/* TODO check if I'm pushing correctly and if the registers are ordered correctly */
static inline void push(int16_t what) {
    memory[stack_pointer - 2] = what & 255; // low bits
    memory[stack_pointer - 1] = (what >> 8); // high bits
    stack_pointer -= 2;
}

static inline void push_b(void) {
    push(regs.BC);
}

static inline void push_d(void) {
    push(regs.DE);
}

static inline void push_h(void) {
    push(regs.HL);
}

static inline void push_psw(void) {
    push(regs.PSW);
}

/* pop 16 bits from the stack into big register; reverse of push */

/* TODO check if B is the high bit */
static inline void pop(uint16_t* where) {
    *where = (memory[stack_pointer + 1] << 8) 
        | memory[stack_pointer];
    stack_pointer += 2;
}

static inline void pop_b(void) {
    pop(&regs.BC);
}

static inline void pop_d(void) {
    pop(&regs.DE);
}

static inline void pop_h(void) {
    pop(&regs.HL);
}

static inline void pop_psw(void) {
    pop(&regs.PSW);
}

/* exchange 16bits from top of stack with HL register */

static inline void xthl(void) {
    swap(regs.L, memory[stack_pointer]);
    swap(regs.H, memory[stack_pointer + 1]);
}

static inline void sphl(void) {
    // TODO check that the byte order is as expected
    stack_pointer = regs.HL;
}

/* equivalent with jmp HL */
static inline void pchl(void) {
    // TODO check that the byte order is as expected
    program_counter = regs.HL;
}

static inline void xchg(void) {
    swap(regs.HL, regs.DE);
}

/* ========================================================== */
/* ===================== flag operation ===================== */
/* ========================================================== */

/* TODO seperate flag setter for AC --> this is only used in DAA */
/* TODO these functions need better names */
static inline void update_flag_ac(void);

/* TODO seperate flag setter for all other flags */
/* TODO maybe rename this. it's a bit confusing */
static inline void update_flags_non_ac(void);

static inline void update_flag_variables(int16_t operand, operation op) {
    last_acc = regs.A;
    last_operand = operand;
    last_operation = op;
}

/* ========================================================== */
/* =================== 8-bit arithmetic ===================== */
/* ========================================================== */

/* add register R to the accumulator -- affects flags Z, S, P, C, AC */

static inline void add(int8_t operand) {
    update_flag_variables(operand, ADD8);
    regs.A += operand;
}

static inline void add_b(void) {
    add(regs.B);
}

static inline void add_c(void) {
    add(regs.C);
}

static inline void add_d(void) {
    add(regs.D);
}

static inline void add_e(void) {
    add(regs.E);
}

static inline void add_h(void) {
    add(regs.H);
}

static inline void add_m(void) {
    add(memory[regs.HL]);
}

static inline void add_a(void) {
    add(regs.A);
}

/* TODO check flags affected in manual */

/* subtract register R from the accumulator -- affects flags Z, S, P, C, AC */

static inline void sub(int8_t operand) {
    update_flag_variables(operand, SUB8);
    regs.A -= operand;
}

static inline void sub_b(void) {
    sub(regs.B);
}

static inline void sub_c(void) {
    sub(regs.C);
}

static inline void sub_d(void) {
    sub(regs.D);
}

static inline void sub_e(void) {
    sub(regs.E);
}

static inline void sub_h(void) {
    sub(regs.H);
}

static inline void sub_m(void) {
    sub(memory[regs.HL]);
}

static inline void sub_a(void) {
    sub(regs.A);
}

/* increase the register of memory area by 1 -- affects flags Z, S, P, -, AC */

static inline void inr(uint8_t *r) {
    update_flag_variables(*r, INR);
    *r += 1;
}

static inline void inr_b(void) {
    inr(&regs.B);
}

static inline void inr_c(void) {
    inr(&regs.C);
}

static inline void inr_d(void) {
    inr(&regs.D);
}

static inline void inr_e(void) {
    inr(&regs.E);
}

static inline void inr_h(void) {
    inr(&regs.H);
}

static inline void inr_l(void) {
    inr(&regs.L);
}

static inline void inr_m(void) {
    inr(&memory[regs.HL]);
}

static inline void inr_a(void) {
    inr(&regs.A);
}

/* decrease the register of memory area by 1 -- affects flags Z, S, P, -, AC */

static inline void dcr(uint8_t *r) {
    update_flag_variables(*r, INR);
    *r -= 1;
}

static inline void dcr_b(void) {
    dcr(&regs.B);
}

static inline void dcr_c(void) {
    dcr(&regs.C);
}

static inline void dcr_d(void) {
    dcr(&regs.D);
}

static inline void dcr_e(void) {
    dcr(&regs.E);
}

static inline void dcr_h(void) {
    dcr(&regs.H);
}

static inline void dcr_l(void) {
    dcr(&regs.L);
}

static inline void dcr_m(void) {
    dcr(&memory[regs.HL]);
}

static inline void dcr_a(void) {
    dcr(&regs.A);
}

/* compares register or memory area with accumulator -- affects flags Z, S, P, C, AC */

static inline void cmp(uint8_t value) {
    update_flag_variables(value, CMP8); 
}

static inline void cmp_b(void) {
    cmp(regs.B);
}

static inline void cmp_c(void) {
    cmp(regs.C);
}

static inline void cmp_d(void) {
    cmp(regs.D);
}

static inline void cmp_e(void) {
    cmp(regs.E);
}

static inline void cmp_h(void) {
    cmp(regs.H);
}

static inline void cmp_l(void) {
    cmp(regs.L);
}

static inline void cmp_m(void) {
    cmp(memory[regs.HL]);
}

static inline void cmp_a(void) {
    cmp(regs.A);
}

/* Logically AND (&) register/memory with accumulator -- flag affected C, Z, S, P */

static inline void ana(int8_t val) {
    regs.A &= val;
    update_flag_variables(val, AND8);
}

static inline void ana_b(void) {
    ana(regs.B);
}

static inline void ana_c(void) {
    ana(regs.C);
}

static inline void ana_d(void) {
    ana(regs.D);
}

static inline void ana_e(void) {
    ana(regs.E);
}

static inline void ana_h(void) {
    ana(regs.H);
}

static inline void ana_l(void) {
    ana(regs.L);
}

static inline void ana_m(void) {
    ana(memory[regs.HL]);
}

static inline void ana_a(void) {
    ana(regs.A);
}

/* Logically OR (|) register/memory with accumulator -- flag affected C, Z, S, P */

static inline void ora(int8_t val) {
    regs.A |= val;
    update_flag_variables(val, OR8);
}

static inline void ora_b(void) {
    ora(regs.B);
}

static inline void ora_c(void) {
    ora(regs.C);
}

static inline void ora_d(void) {
    ora(regs.D);
}

static inline void ora_e(void) {
    ora(regs.E);
}

static inline void ora_h(void) {
    ora(regs.H);
}

static inline void ora_l(void) {
    ora(regs.L);
}

static inline void ora_m(void) {
    ora(memory[regs.HL]);
}

static inline void ora_a(void) {
    ora(regs.A);
}

/* Logically XOR (^) register/memory with accumulator -- flag affected C, Z, S, ?, P */

static inline void xra(int8_t val) {
    regs.A ^= val;
    update_flag_variables(val, XOR8);
}

static inline void xra_b(void) {
    xra(regs.B);
}

static inline void xra_c(void) {
    xra(regs.C);
}

static inline void xra_d(void) {
    xra(regs.D);
}

static inline void xra_e(void) {
    xra(regs.E);
}

static inline void xra_h(void) {
    xra(regs.H);
}

static inline void xra_l(void) {
    xra(regs.L);
}

static inline void xra_m(void) {
    xra(memory[regs.HL]);
}

static inline void xra_a(void) {
    xra(regs.A);
}

/* immediate 8-bit arithmetic operations */

static inline void adi(void) {
    int8_t operand = get_op_byte(1);
    update_flag_variables(operand, ADD8);
    regs.A += operand;
}

static inline void sui(void) {
    int8_t operand = get_op_byte(1);
    update_flag_variables(operand, SUB8);
    regs.A -= operand;
}

static inline void cpi(void) {
    int8_t operand = get_op_byte(1);
    update_flag_variables(operand, CMP8);
}

static inline void ani(void) {
    // TODO correct upstream typo (https://gist.github.com/joefg/634fa4a1046516d785c9#file-8080-op-L52)
    int8_t operand = get_op_byte(1);
    update_flag_variables(operand, AND8);
    regs.A &= operand;
}

static inline void ori(void) {
    int8_t operand = get_op_byte(1);
    update_flag_variables(operand, OR8);
    regs.A |= operand;
}

static inline void xri(void) {
    int8_t operand = get_op_byte(1);
    update_flag_variables(operand, XOR8);
    regs.A ^= operand;
}

static inline void daa(void) {
    /* TODO */
}

/* add register or memory to accumulator with carry - affects C, S, Z, P, AC */

static inline void adc(int8_t operand) {
    /* TODO is the order here is correct */
    update_flags_non_ac();
    update_flag_variables(operand, ADC);
    regs.A += operand + regs.cf;
}

static inline void adc_b(void) {
    adc(regs.B);
}

static inline void adc_c(void) {
    adc(regs.C);
}

static inline void adc_d(void) {
    adc(regs.D);
}

static inline void adc_e(void) {
    adc(regs.E);
}

static inline void adc_h(void) {
    adc(regs.H);
}

static inline void adc_l(void) {
    adc(regs.L);
}

static inline void adc_m(void) {
    adc(memory[regs.HL]);
}

static inline void adc_a(void) {
    adc(regs.A);
}

/* add immediate value to accumulator with carry - affects C, S, Z, P, AC */

static inline void aci(void) {
    int8_t value = get_op_byte(1);
    update_flags_non_ac();
    update_flag_variables(value, ADC);
    regs.A += value + regs.cf;
}

/* subtract register or memory from accumulator with carry - affects C, S, Z, P, AC */

static inline void sbb(int8_t operand) {
    update_flags_non_ac();
    update_flag_variables(operand, SBB);
    regs.A -= (operand + regs.cf);
}

static inline void sbb_b(void) {
    sbb(regs.B);
}

static inline void sbb_c(void) {
    sbb(regs.C);
}

static inline void sbb_d(void) {
    sbb(regs.D);
}

static inline void sbb_e(void) {
    sbb(regs.E);
}

static inline void sbb_h(void) {
    sbb(regs.H);
}

static inline void sbb_l(void) {
    sbb(regs.L);
}

static inline void sbb_m(void) {
    sbb(memory[regs.HL]);
}

static inline void sbb_a(void) {
    sbb(regs.A);
}

/* subtract immediate value from accumulator with carry - affects C, S, Z, P, AC */

static inline void sbi(void) {
    int8_t value = get_op_byte(1);
    update_flags_non_ac();
    update_flag_variables(value, SBB);
    regs.A -= (value + regs.cf);
}

/* ========================================================== */
/* =================== 16-bit arithmetic ==================== */
/* ========================================================== */

/* adds content of register-pair to HL */

static inline void dad(int16_t operand) {
    update_flag_variables(operand, DAD16);
    regs.HL += operand;
}

static inline void dad_b(void) {
    dad(regs.BC);
}

static inline void dad_d(void) {
    dad(regs.DE);
}

static inline void dad_h(void) {
    dad(regs.HL);
}

static inline void dad_sp(void) {
    dad(stack_pointer);
}

/* increments register-pair by 1 */

static inline void inx_b(void) {
    regs.BC += 1;
}

static inline void inx_d(void) {
    regs.DE += 1;
}

static inline void inx_h(void) {
    regs.HL += 1;
}

static inline void inx_sp(void) {
    stack_pointer += 1;
}

/* decrements register-pair by 1 */

static inline void dcx_b(void) {
    regs.BC -= 1;
}

static inline void dcx_d(void) {
    regs.DE -= 1;
}

static inline void dcx_h(void) {
    regs.HL -= 1;
}

static inline void dcx_sp(void) {
    stack_pointer -= 1;
}

/* ========================================================== */
/* =================== Jumps, Calls, Rets =================== */
/* ========================================================== */

/* get 16-bit address from operation and move PC there */
static inline void jmp(void) {
    int16_t addr = get_op_address();
    program_counter = addr;
}

/* jump if the zero bit is not set */
static inline void jnz(void) {
    update_flags_non_ac();
    if (!regs.zf) {
        int16_t addr = get_op_address();
        program_counter = addr;
    }
}

/* jump if the zero bit is set */
static inline void jz(void) {
    update_flags_non_ac();
    if (regs.zf) {
        int16_t addr = get_op_address();
        program_counter = addr;
    }
}

/* jump if the carry bit is not set */
static inline void jnc(void) {
    update_flags_non_ac();
    if (!regs.cf) {
        int16_t addr = get_op_address();
        program_counter = addr;
    }
}

/* jump if the carry bit is set */
static inline void jc(void) {
    update_flags_non_ac();
    if (regs.cf) {
        int16_t addr = get_op_address();
        program_counter = addr;
    }
}

/* jump if the parity bit is not set - odd */
static inline void jpo(void) {
    update_flags_non_ac();
    if (!regs.pa) {
        int16_t addr = get_op_address();
        program_counter = addr;
    }
}

/* jump if the parity bit is set - even */
static inline void jpe(void) {
    update_flags_non_ac();
    if (regs.pa) {
        int16_t addr = get_op_address();
        program_counter = addr;
    }
}

/* jump if the sign bit is not set - plus */
static inline void jp(void) {
    update_flags_non_ac();
    if (!regs.sf) {
        int16_t addr = get_op_address();
        program_counter = addr;
    }
}

/* jump if the sign bit is set - minus */
static inline void jm(void) {
    update_flags_non_ac();
    if (regs.sf) {
        int16_t addr = get_op_address();
        program_counter = addr;
    }
}

/* push address on the stack for ret and move PC there */
static inline void call(void) {
    int16_t addr = get_op_address();
    push(addr);
    program_counter = addr;
}

/* call if carry flag is set */
static inline void cc(void) {
    update_flags_non_ac();
    if (regs.cf) {
        call();
    }
}

/* call if carry flag is not set */
static inline void cnc(void) {
    update_flags_non_ac();
    if (!regs.cf) {
        call();
    }
}

/* call if zero */
static inline void cz(void) {
    update_flags_non_ac();
    if (regs.zf) {
        call();
    }
}

/* call if non-zero */
static inline void cnz(void) {
    update_flags_non_ac();
    if (!regs.zf) {
        call();
    }
}

/* call if negative */
static inline void cm(void) {
    update_flags_non_ac();
    if (regs.sf) {
        call();
    }
}

/* call if positive */
static inline void cp(void) {
    update_flags_non_ac();
    if (!regs.sf) {
        call();
    }
}

/* call if parity is even */
static inline void cpe(void) {
    update_flags_non_ac();
    if (regs.pa) {
        call();
    }
}

/* call if parity is odd */
static inline void cpo(void) {
    update_flags_non_ac();
    if (!regs.pa) {
        call();
    }
}

/* opposite of ret; pop address from stack and move PC there */
static inline void ret(void) {
    uint16_t addr;
    pop(&addr);
    program_counter = addr;
}

/* return if the carry bit is set */
static inline void rc(void) {
    update_flags_non_ac();
    if (regs.cf) {
        ret();
    }
}

/* return if the carry bit is not set */
static inline void rnc(void) {
    update_flags_non_ac();
    if (!regs.cf) {
        ret();
    }
}

/* return if the zero bit is set */
static inline void rz(void) {
    update_flags_non_ac();
    if (regs.zf) {
        ret();
    }
}

/* return if the zero bit is not set */
static inline void rnz(void) {
    update_flags_non_ac();
    if (!regs.zf) {
        ret();
    }
}

/* return if the sign bit is set - minus */
static inline void rm(void) {
    update_flags_non_ac();
    if (regs.sf) {
        ret();
    }
}

/* return if the sign bit is not set - plus */
static inline void rp(void) {
    update_flags_non_ac();
    if (!regs.sf) {
        ret();
    }
}

/* return if the parity bit is set - even */
static inline void rpe(void) {
    update_flags_non_ac();
    if (regs.pa) {
        ret();
    }
}

/* return if the parity bit is not set - odd */
static inline void rpo(void) {
    update_flags_non_ac();
    if (!regs.pa) {
        ret();
    }
}

/* ========================================================== */
/* ======================== Rotations ======================= */
/* ========================================================== */

/* rotate accumulator left throught CF */
static inline void ral(void) {
    update_flags_non_ac();
    uint8_t old_cf = regs.cf;
    regs.cf = regs.A & (1 << 7);
    regs.A = (regs.A << 1) | old_cf;
}

/* rotate accumulator right throught CF */
static inline void rar(void) {
    update_flags_non_ac();
    uint8_t old_lb = regs.A & 1;
    regs.A = (regs.A >> 1) | (regs.cf << 7);
    regs.cf = old_lb;
}

/* rotate accumulator left; CF becomes higher order bit */
static inline void rlc(void) {
    update_flags_non_ac();
    regs.cf = regs.A & (1 << 7);
    regs.A = (regs.A << 1) | (regs.A & (1 << 7));
}

/* rotate accumulator right; CF becomes lower order bit */
static inline void rrc(void) {
    update_flags_non_ac();
    regs.cf = regs.A & 1;
    regs.A = (regs.A >> 1) | ((regs.A & 1) << 7);
}

/* TODO have this in a sepparate file, so all operation
 * implementations are isolated */

op_code_detail op_code_details[OP_CODES_CNT] = {
    {1,  4, 0, &nop},  // NOP
    {3, 10, 0, &lxi_b}, // LXI_B_D16
    {1,  7, 0, &stax_b}, // STAX_B
    {1,  5, 0, &inx_b}, // INX_B
    {1,  5, 0, &inr_b}, // INR_B
    {1,  5, 0, &dcr_b}, // DCR_B
    {2,  7, 0, &mvi_b}, // MVI_B_D8
    {1,  4, 0, OP_MISC}, // RLC
    {1,  4, 0, &nop},  // NOP_DUP_0
    {1, 10, 0, &dad_b}, // DAD_B
    {1,  7, 0, &ldax_b}, // LDAX_B
    {1,  5, 0, &dcx_b}, // DCX_B
    {1,  5, 0, &inr_c}, // INR_C
    {1,  5, 0, &dcr_c}, // DCR_C
    {2,  7, 0, &mvi_c}, // MVI_C_D8
    {1,  4, 0, OP_MISC}, // RRC
    {1,  4, 0, &nop},  // NOP_DUP_1
    {3, 10, 0, &lxi_d}, // LXI_D_D16
    {1,  7, 0, &stax_d}, // STAX_D
    {1,  5, 0, &inx_d}, // INX_D
    {1,  5, 0, &inr_d}, // INR_D
    {1,  5, 0, &dcr_d}, // DCR_D
    {2,  7, 0, &mvi_d}, // MVI_D
    {1,  4, 0, OP_MISC}, // RAL
    {1,  4, 0, &nop},  // NOP_DUP_2
    {1, 10, 0, &dad_d}, // DAD_D
    {1,  7, 0, &ldax_d}, // LDAX_D
    {1,  5, 0, &dcx_d}, // DCX_D
    {1,  5, 0, &inr_e}, // INR_E
    {1,  5, 0, &dcr_e}, // DCR_E
    {2,  7, 0, &mvi_e}, // MVI_E_D8
    {1,  4, 0, OP_MISC}, // RAR
    {1,  4, 0, &nop},  // NOP_DUP_3
    {3, 10, 0, &lxi_h}, // LXI_H_D16
    {3, 16, 0, &shld}, // SHLD_A16
    {1,  5, 0, &inx_h}, // INX_H
    {1,  5, 0, &inr_h}, // INR_H
    {1,  5, 0, &dcr_h}, // DCR_H
    {2,  7, 0, &mvi_h}, // MVI_H_D8
    {1,  4, 0, OP_MISC}, // DAA
    {1,  4, 0, &nop},  // NOP_DUP_4
    {1, 10, 0, &dad_h}, // DAD_H
    {3, 16, 0, &lhld}, // LHLD_A16
    {1,  5, 0, &dcx_h}, // DCX_H
    {1,  5, 0, &inr_l}, // INR_L
    {1,  5, 0, &dcr_l}, // DCR_L
    {2,  7, 0, &mvi_l}, // MVI_L_D8
    {1,  4, 0, OP_MISC}, // CMA
    {1,  4, 0, &nop},  // NOP_DUP_5
    {3, 10, 0, &lxi_sp}, // LXI_SP_D16
    {3, 13, 0, &sta}, // STA_A16
    {1,  5, 0, &inx_sp}, // INX_SP
    {1, 10, 0, &inr_m}, // INR_M
    {1, 10, 0, &dcr_m}, // DCR_M
    {2, 10, 0, &mvi_m}, // MVI_M_D8
    {1,  4, 0, OP_MISC}, // STC
    {1,  4, 0, &nop},  // NOP_DUP_6
    {1, 10, 0, &dad_sp}, // DAD_SP
    {3, 13, 0, &lda}, // LDA_A16
    {1,  5, 0, &dcx_sp}, // DCX_SP
    {1,  5, 0, &inr_a}, // INR_A
    {1,  5, 0, &dcr_a}, // DCR_A
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
    {1,  4, 0, &add_b}, // ADD_B
    {1,  4, 0, &add_c}, // ADD_C
    {1,  4, 0, &add_d}, // ADD_D
    {1,  4, 0, &add_e}, // ADD_E
    {1,  4, 0, &add_h}, // ADD_H
    {1,  4, 0, &add_l}, // ADD_L
    {1,  7, 0, &add_m}, // ADD_M
    {1,  4, 0, &add_a}, // ADD_A
    {1,  4, 0, &adc_b}, // ADC_B
    {1,  4, 0, &adc_c}, // ADC_C
    {1,  4, 0, &adc_d}, // ADC_D
    {1,  4, 0, &adc_e}, // ADC_E
    {1,  4, 0, &adc_h}, // ADC_H
    {1,  4, 0, &adc_l}, // ADC_L
    {1,  7, 0, &adc_m}, // ADC_M
    {1,  4, 0, &adc_a}, // ADC_A
    {1,  4, 0, &sub_b}, // SUB_B
    {1,  4, 0, &sub_c}, // SUB_C
    {1,  4, 0, &sub_d}, // SUB_D
    {1,  4, 0, &sub_e}, // SUB_E
    {1,  4, 0, &sub_h}, // SUB_H
    {1,  4, 0, &sub_l}, // SUB_L
    {1,  7, 0, &sub_m}, // SUB_M
    {1,  4, 0, &sub_a}, // SUB_A
    {1,  4, 0, &sbb_b}, // SBB_B
    {1,  4, 0, &sbb_c}, // SBB_C
    {1,  4, 0, &sbb_d}, // SBB_D
    {1,  4, 0, &sbb_e}, // SBB_E
    {1,  4, 0, &sbb_h}, // SBB_H
    {1,  4, 0, &sbb_l}, // SBB_L
    {1,  7, 0, &sbb_m}, // SBB_M
    {1,  4, 0, &sbb_a}, // SBB_A
    {1,  4, 0, &ana_b}, // ANA_B
    {1,  4, 0, &ana_c}, // ANA_C
    {1,  4, 0, &ana_d}, // ANA_D
    {1,  4, 0, &ana_e}, // ANA_E
    {1,  4, 0, &ana_h}, // ANA_H
    {1,  4, 0, &ana_l}, // ANA_L
    {1,  7, 0, &ana_m}, // ANA_M
    {1,  4, 0, &ana_a}, // ANA_A
    {1,  4, 0, &xra_b}, // XRA_B
    {1,  4, 0, &xra_c}, // XRA_C
    {1,  4, 0, &xra_d}, // XRA_D
    {1,  4, 0, &xra_e}, // XRA_E
    {1,  4, 0, &xra_h}, // XRA_H
    {1,  4, 0, &xra_l}, // XRA_L
    {1,  7, 0, &xra_m}, // XRA_M
    {1,  4, 0, &xra_a}, // XRA_A
    {1,  4, 0, &ora_b}, // ORA_B
    {1,  4, 0, &ora_c}, // ORA_C
    {1,  4, 0, &ora_d}, // ORA_D
    {1,  4, 0, &ora_e}, // ORA_E
    {1,  4, 0, &ora_h}, // ORA_H
    {1,  4, 0, &ora_l}, // ORA_L
    {1,  7, 0, &ora_m}, // ORA_M
    {1,  4, 0, &ora_a}, // ORA_A
    {1,  4, 0, &cmp_b}, // CMP_B
    {1,  4, 0, &cmp_c}, // CMP_C
    {1,  4, 0, &cmp_d}, // CMP_D
    {1,  4, 0, &cmp_e}, // CMP_E
    {1,  4, 0, &cmp_h}, // CMP_H
    {1,  4, 0, &cmp_l}, // CMP_L
    {1,  7, 0, &cmp_m}, // CMP_M
    {1,  4, 0, &cmp_a}, // CMP_A
    {1,  11,5, &rnz}, // RNZ
    {1, 10, 0, &pop_b}, // POP_B
    {3, 10, 0, &jnz}, // JNZ_A16
    {3, 10, 0, &jmp}, // JMP_A16
    {3, 17,11, &cnz}, // CNZ_A16
    {1, 11, 0, &push_b}, // PUSH_B
    {2,  7, 0, &adi}, // ADI_D8
    {1, 11, 0, OP_MISC}, // RST_0
    {1, 11, 5, &rz}, // RZ
    {1, 10, 0, &ret}, // RET
    {3, 10, 0, &jz}, // JZ_A16
    {3, 10, 0, &jmp}, // JMP_A16_DUP_0
    {3, 17,11, &cz}, // CZ
    {3, 17, 0, &call}, // CALL_A16
    {2,  7, 0, &aci}, // ACI_D8
    {1, 11, 0, OP_MISC}, // RST_1
    {1, 11, 5, &rnc}, // RNC
    {1, 10, 0, &pop_d}, // POP_D
    {3, 10, 0, &jnc}, // JNC_A16
    {2, 10, 0, OP_MISC}, // OUT_D8
    {3, 17,11, &cnc}, // CNC_A16
    {1, 11, 0, &push_d}, // PUSH_D
    {2,  7, 0, &sui}, // SUI_D8
    {1, 11, 0, OP_MISC}, // RST_2
    {1,  11,5, &rc}, // RC
    {1, 10, 0, OP_MISC}, // RET_DUP_0
    {3, 10, 0, &jc}, // JC_A16
    {2, 10, 0, OP_MISC}, // IN_D8
    {3, 17,11, &cc}, // CC_A16
    {3, 17, 0, &call}, // CALL_A16_DUP_0
    {2,  7, 0, &sbi}, // SBI_D8
    {1, 11, 0, OP_MISC}, // RST_3
    {1, 11, 5, &rpo}, // RPO
    {1, 10, 0, &pop_h}, // POP_H
    {3, 10, 0, &jpo}, // JPO_A16
    {1, 18, 0, &xthl}, // XTHL
    {3, 17,11, &cpo}, // CPO_A16
    {1, 11, 0, &push_h}, // PUSH_H
    {2,  7, 0, &ani}, // ANI_D8
    {1, 11, 0, OP_MISC}, // RST_4
    {1, 11, 5, &rpe}, // RPE
    {1,  5, 0, &pchl}, // PCHL
    {3, 10, 0, &jpe}, // JPE_A16
    {1,  5, 0, &xchg}, // XCHG
    {3, 17,11, &cpe}, // CPE_A16
    {3, 17, 0, &call}, // CALL_A16_DUP_1
    {2,  7, 0, &xri}, // XRI_D8
    {1, 11, 0, OP_MISC}, // RST_5
    {1, 11, 5, &rp}, // RP
    {1, 10, 0, &pop_psw}, // POP_PSW
    {3, 10, 0, &jp}, // JP_A16
    {1,  4, 0, OP_MISC}, // DI
    {3, 17,11, &cp}, // CP_A16
    {1, 11, 0, &push_psw}, // PUSH_PSW
    {2,  7, 0, &ori}, // ORI_D8
    {1, 11, 0, OP_MISC}, // RST_6
    {1,  11,5, &rm}, // RM
    {1,  5, 0, &sphl}, // SPHL
    {3, 10, 0, &jm}, // JM_A16
    {1,  4, 0, OP_MISC}, // EI
    {3, 17,11, &cm}, // CM_A16
    {3, 17, 0, &call}, // CALL_A16_DUP_2
    {2,  7, 0, &cpi}, // CPI_D8
    {1, 11, 0, OP_MISC}, // RST_7
};

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

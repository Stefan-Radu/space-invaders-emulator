#include <stdint.h>
#include <stdio.h>

#include "cpu_8080.h"

uint8_t  memory[0x10000];

uint16_t stack_pointer;
uint16_t program_counter;

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

// ============ INSTRUCTIONS ============

// TODO lazy flag evaluation

/* nop */
inline void nop(void) {}

/* ========================================================== */

/* 8-bit transfers */

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
inline void mov_b_b(void) { mov_reg_reg(&regs.B, regs.B); }
inline void mov_b_c(void) { mov_reg_reg(&regs.B, regs.C); }
inline void mov_b_d(void) { mov_reg_reg(&regs.B, regs.D); }
inline void mov_b_e(void) { mov_reg_reg(&regs.B, regs.E); }
inline void mov_b_h(void) { mov_reg_reg(&regs.B, regs.H); }
inline void mov_b_l(void) { mov_reg_reg(&regs.B, regs.L); }
inline void mov_b_m(void) { mov_reg_mem(&regs.B); }
inline void mov_b_a(void) { mov_reg_reg(&regs.B, regs.A); }

/* target: C */
inline void mov_c_b(void) { mov_reg_reg(&regs.C, regs.B); }
inline void mov_c_c(void) { mov_reg_reg(&regs.C, regs.C); }
inline void mov_c_d(void) { mov_reg_reg(&regs.C, regs.D); }
inline void mov_c_e(void) { mov_reg_reg(&regs.C, regs.E); }
inline void mov_c_h(void) { mov_reg_reg(&regs.C, regs.H); }
inline void mov_c_l(void) { mov_reg_reg(&regs.C, regs.L); }
inline void mov_c_m(void) { mov_reg_mem(&regs.C); }
inline void mov_c_a(void) { mov_reg_reg(&regs.C, regs.A); }

/* target: D */
inline void mov_d_b(void) { mov_reg_reg(&regs.D, regs.B); }
inline void mov_d_c(void) { mov_reg_reg(&regs.D, regs.C); }
inline void mov_d_d(void) { mov_reg_reg(&regs.D, regs.D); }
inline void mov_d_e(void) { mov_reg_reg(&regs.D, regs.E); }
inline void mov_d_h(void) { mov_reg_reg(&regs.D, regs.H); }
inline void mov_d_l(void) { mov_reg_reg(&regs.D, regs.L); }
inline void mov_d_m(void) { mov_reg_mem(&regs.D); }
inline void mov_d_a(void) { mov_reg_reg(&regs.D, regs.A); }

/* target: E */
inline void mov_e_b(void) { mov_reg_reg(&regs.E, regs.B); }
inline void mov_e_c(void) { mov_reg_reg(&regs.E, regs.C); }
inline void mov_e_d(void) { mov_reg_reg(&regs.E, regs.D); }
inline void mov_e_e(void) { mov_reg_reg(&regs.E, regs.E); }
inline void mov_e_h(void) { mov_reg_reg(&regs.E, regs.H); }
inline void mov_e_l(void) { mov_reg_reg(&regs.E, regs.L); }
inline void mov_e_m(void) { mov_reg_mem(&regs.E); }
inline void mov_e_a(void) { mov_reg_reg(&regs.E, regs.A); }

/* target: F */
inline void mov_h_b(void) { mov_reg_reg(&regs.H, regs.B); }
inline void mov_h_c(void) { mov_reg_reg(&regs.H, regs.C); }
inline void mov_h_d(void) { mov_reg_reg(&regs.H, regs.D); }
inline void mov_h_e(void) { mov_reg_reg(&regs.H, regs.E); }
inline void mov_h_h(void) { mov_reg_reg(&regs.H, regs.H); }
inline void mov_h_l(void) { mov_reg_reg(&regs.H, regs.L); }
inline void mov_h_m(void) { mov_reg_mem(&regs.H); }
inline void mov_h_a(void) { mov_reg_reg(&regs.H, regs.A); }

/* target: L */
inline void mov_l_b(void) { mov_reg_reg(&regs.L, regs.B); }
inline void mov_l_c(void) { mov_reg_reg(&regs.L, regs.C); }
inline void mov_l_d(void) { mov_reg_reg(&regs.L, regs.D); }
inline void mov_l_e(void) { mov_reg_reg(&regs.L, regs.E); }
inline void mov_l_h(void) { mov_reg_reg(&regs.L, regs.H); }
inline void mov_l_l(void) { mov_reg_reg(&regs.L, regs.L); }
inline void mov_l_m(void) { mov_reg_mem(&regs.L); }
inline void mov_l_a(void) { mov_reg_reg(&regs.L, regs.A); }

/* target: memory */
inline void mov_m_b(void) { mov_mem_reg(regs.B); }
inline void mov_m_c(void) { mov_mem_reg(regs.C); }
inline void mov_m_d(void) { mov_mem_reg(regs.D); }
inline void mov_m_e(void) { mov_mem_reg(regs.E); }
inline void mov_m_h(void) { mov_mem_reg(regs.H); }
inline void mov_m_l(void) { mov_mem_reg(regs.L); }
/* void hlt in category misc */
inline void mov_m_a(void) { mov_mem_reg(regs.A); }

/* target: A */
inline void mov_a_b(void) { mov_reg_reg(&regs.A, regs.B); }
inline void mov_a_c(void) { mov_reg_reg(&regs.A, regs.C); }
inline void mov_a_d(void) { mov_reg_reg(&regs.A, regs.D); }
inline void mov_a_e(void) { mov_reg_reg(&regs.A, regs.E); }
inline void mov_a_h(void) { mov_reg_reg(&regs.A, regs.H); }
inline void mov_a_l(void) { mov_reg_reg(&regs.A, regs.L); }
inline void mov_a_m(void) { mov_reg_mem(&regs.A); }
inline void mov_a_a(void) { mov_reg_reg(&regs.A, regs.A); }

/* ========================================================== */

/*// 16-bit transfers*/
/*#define LXI(reg, val) (registers.(reg) = val)*/

/*// 8-bit arithmetic*/
/*#define INR(reg) registers.(reg) += 1\*/
                /*if (registers.(reg) == 0) registers.zf = 1; \*/
                /*if (registers.(reg) == 0) registers.cf = 1; \*/
                /*if (registers.(reg) & (1 << 7)) registers.sf = 1; \*/
                /*if (*/
            


/*// 16-bit arithmetic*/
/*#define INX(reg) (registers.(reg) += 1)*/


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

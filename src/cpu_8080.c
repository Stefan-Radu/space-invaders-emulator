#include <stdint.h>
#include <stdio.h>

#include "cpu_8080.h"
#include "opcodes.h"


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
//
//
//
// TODO think of how to implement instructions
// TODO lazy flag evaluation

/*#define NOP*/

/*// 8-bit transfers*/
/*#define STAX(reg_dst, reg_src) (store(registers.reg_dst, reg_src))*/

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

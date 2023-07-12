#include <stdint.h>
#include <stdio.h>

#include "cpu_8080.h"


uint16_t stack_pointer;
uint16_t program_counter;

typedef union {
    struct {    
        // flags
        uint8_t F; uint8_t A;
        uint8_t C; uint8_t B; 
        uint8_t E; uint8_t D; 
        uint8_t L; uint8_t H; 
    };
    struct {
        uint16_t PSW; // A|F (program status word)
        uint16_t BC;  // (B|C) (reffered to as B)
        uint16_t DE;  // (D|E) (reffered to as D)
        uint16_t HL;  // (H|L) (reffered to as H)
    };
} registers;

static registers regs;

static void test_set_registers() {
    regs.A = 10;
    regs.B = 20;
    regs.D = 30;
    regs.H = 40;
    regs.F = 1;
    regs.C = 2;
    regs.E = 3;
    regs.L = 4;

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


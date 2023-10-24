#ifndef CPU_8080_H
#define CPU_8080_H

#include <stdint.h>

void print_registers(void);

/* nop */
inline void nop(void);

/* ======================= */

/* 8-bit transfers */
inline void mov_b_b(void);
inline void mov_b_c(void);
inline void mov_b_d(void);
inline void mov_b_e(void);
inline void mov_b_h(void);
inline void mov_b_l(void);
inline void mov_b_m(void);
inline void mov_b_a(void);
inline void mov_c_b(void);
inline void mov_c_c(void);
inline void mov_c_d(void);
inline void mov_c_e(void);
inline void mov_c_h(void);
inline void mov_c_l(void);
inline void mov_c_m(void);
inline void mov_c_a(void);
inline void mov_d_b(void);
inline void mov_d_c(void);
inline void mov_d_d(void);
inline void mov_d_e(void);
inline void mov_d_h(void);
inline void mov_d_l(void);
inline void mov_d_m(void);
inline void mov_d_a(void);
inline void mov_e_b(void);
inline void mov_e_c(void);
inline void mov_e_d(void);
inline void mov_e_e(void);
inline void mov_e_h(void);
inline void mov_e_l(void);
inline void mov_e_m(void);
inline void mov_e_a(void);
inline void mov_h_b(void);
inline void mov_h_c(void);
inline void mov_h_d(void);
inline void mov_h_e(void);
inline void mov_h_h(void);
inline void mov_h_l(void);
inline void mov_h_m(void);
inline void mov_h_a(void);
inline void mov_l_b(void);
inline void mov_l_c(void);
inline void mov_l_d(void);
inline void mov_l_e(void);
inline void mov_l_h(void);
inline void mov_l_l(void);
inline void mov_l_m(void);
inline void mov_l_a(void);
inline void mov_m_b(void);
inline void mov_m_c(void);
inline void mov_m_d(void);
inline void mov_m_e(void);
inline void mov_m_h(void);
inline void mov_m_l(void);
/* void hlt in misc */
inline void mov_m_a(void);
inline void mov_a_b(void);
inline void mov_a_c(void);
inline void mov_a_d(void);
inline void mov_a_e(void);
inline void mov_a_h(void);
inline void mov_a_l(void);
inline void mov_a_m(void);
inline void mov_a_a(void);

/* ======================= */

#endif

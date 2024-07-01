#ifndef INSTR_H
#define INSTR_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

extern uint32_t pc;
extern uint32_t x[32];
extern uint32_t instr;
extern int64_t brojac;
extern uint64_t ukupan_broj;

//#define PRINT (ukupan_broj % 1000000 == 0)
//#define PRINT (ukupan_broj > 13200000)
#define PRINT 0

uint32_t sext(uint32_t val, uint32_t bits);

// rv32i
void lui(void);
void auipc(void);
void addi(void);
void slti(void);
void sltiu(void);
void xori(void);
void ori(void);
void andi(void);
void slli(void);
void srli(void);
void srai(void);
void add(void);
void sub(void);
void sll(void);
void slt(void);
void sltu(void);
void xor (void);
void srl(void);
void sra(void);
void or (void);
void and (void);
void fence(void);
void fencei(void);
void csrrw(void);
void csrrs(void);
void csrrc(void);
void csrrwi(void);
void csrrsi(void);
void csrrci(void);
void ecall(void);
void ebreak(void);
void uret(void);
void sret(void);
void mret(void);
void wfi(void);
void sfencevma(void);
void lb(void);
void lh(void);
void lw(void);
void lbu(void);
void lhu(void);
void sb(void);
void sh(void);
void sw(void);
void jal(void);
void jalr(void);
void beq(void);
void bne(void);
void blt(void);
void bge(void);
void bltu(void);
void bgeu(void);

// rv32m
void mul(void);
void mulh(void);
void mulhsu(void);
void mulhu(void);
void _div(void);
void divu(void);
void rem(void);
void remu(void);

void imm_arith_instr(void);
void arith_instr(void);
void unimplemented(void);
void load(void);
void store(void);
void jump(void);
void exec_instr(void);

#endif

#ifndef INSTR_H
#define INSTR_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

extern uint32_t pc;
extern uint32_t x[32];
extern uint32_t instr;
extern uint64_t brojac;
extern uint64_t ukupan_broj;

#define PRINT (ukupan_broj % 1000000 == 0)
//#define PRINT 1

// rv32i
void lui();
void auipc();
void addi();
void slti();
void sltiu();
void xori();
void ori();
void andi();
void slli();
void srli();
void srai();
void add();
void sub();
void sll();
void slt();
void sltu();
void xor ();
void srl();
void sra();
void or ();
void and ();
void fence();
void fencei();
void csrrw();
void csrrs();
void csrrc();
void csrrwi();
void csrrsi();
void csrrci();
void ecall();
void ebreak();
void uret();
void sret();
void mret();
void wfi();
void sfencevma();
void lb();
void lh();
void lw();
void lbu();
void lhu();
void sb();
void sh();
void sw();
void jal();
void jalr();
void beq();
void bne();
void blt();
void bge();
void bltu();
void bgeu();

// rv32m
void mul();
void mulh();
void mulhsu();
void mulhu();
void _div();
void divu();
void rem();
void remu();

void imm_arith_instr();
void arith_instr();
void unimplemented();
void load();
void store();
void jump();
void exec_instr();

#endif

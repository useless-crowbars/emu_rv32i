#include "instr.h"
#include "mem.h"
#include "reg.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t pc = 0x0;
uint32_t x[32];
uint32_t instr;
int64_t brojac = 0;
uint64_t ukupan_broj = 0;

uint32_t sext(uint32_t val, uint32_t bits)
{
	if (bits >= 32) {
		return val;
	}
	if (val & (1 << (bits - 1))) {
		return val | (0xFFFFFFFF << bits);
	}
	return val;
}

void lui(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	uint32_t imm = instr >> 12;

	if (PRINT) {
		printf("lui %s, %x\n", regs[rd], imm);
	}

	imm = sext(imm, 20);
	x[rd] = imm << 12;
}

void auipc(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	uint32_t imm = instr >> 12;

	if (PRINT) {
		printf("auipc %s, %d\n", regs[rd], imm);
	}

	imm = sext(imm, 20);
	x[rd] = pc - 4 + (imm << 12);
}

void addi(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t imm = instr >> 20;
	imm = sext(imm, 12);

	if (PRINT) {
		printf("addi %s, %s, %d\n", regs[rd], regs[rs1], imm);
	}
	if (rd == 2) {
		if ((int32_t)imm < 0) {
			if (PRINT) {
				printf("USAO %ld\n", brojac);
			}
			brojac++;
		} else {
			brojac--;
			if (PRINT) {
				printf("IZASAO %ld\n", brojac);
			}
		}
	}

	x[rd] = x[rs1] + imm;
}

void slti(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t imm = instr >> 20;
	imm = sext(imm, 12);

	if (PRINT) {
		printf("slti %s, %s, %d\n", regs[rd], regs[rs1], imm);
	}

	x[rd] = (int32_t)x[rs1] < (int32_t)imm;
}

void sltiu(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t imm = instr >> 20;
	imm = sext(imm, 12);

	if (PRINT) {
		printf("sltiu %s, %s, %d\n", regs[rd], regs[rs1], imm);
	}

	x[rd] = x[rs1] < imm;
}

void xori(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t imm = instr >> 20;
	imm = sext(imm, 12);

	if (PRINT) {
		printf("xori %s, %s, %d\n", regs[rd], regs[rs1], imm);
	}

	x[rd] = x[rs1] ^ imm;
}

void ori(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t imm = instr >> 20;
	imm = sext(imm, 12);

	if (PRINT) {
		printf("ori %s, %s, %d\n", regs[rd], regs[rs1], imm);
	}

	x[rd] = x[rs1] | imm;
}

void andi(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t imm = instr >> 20;
	imm = sext(imm, 12);

	if (PRINT) {
		printf("andi %s, %s, %d\n", regs[rd], regs[rs1], imm);
	}

	x[rd] = x[rs1] & imm;
}

void slli(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t shamt = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("slli %s, %s, %x\n", regs[rd], regs[rs1], shamt);
	}

	x[rd] = x[rs1] << shamt;
}

void srli(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t shamt = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("srli %s, %s, %x\n", regs[rd], regs[rs1], shamt);
	}

	x[rd] = x[rs1] >> shamt;
}

void srai(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t shamt = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("srai %s, %s, %x\n", regs[rd], regs[rs1], shamt);
	}

	x[rd] = (uint32_t)((int32_t)x[rs1] >> shamt);
}

void add(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("add %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	x[rd] = x[rs1] + x[rs2];
}

void sub(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("sub %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	x[rd] = x[rs1] - x[rs2];
}

void sll(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("sll %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	x[rd] = x[rs1] << (x[rs2] & 0x1f);
}

void slt(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("slt %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	x[rd] = (int32_t)x[rs1] < (int32_t)x[rs2];
}

void sltu(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("sltu %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	x[rd] = x[rs1] < x[rs2];
}

void xor(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("xor %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	x[rd] = x[rs1] ^ x[rs2];
}

void srl(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("srl %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	x[rd] = x[rs1] >> (x[rs2] & 0x1f);
}

void sra(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("sra %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	x[rd] = (uint32_t)((int32_t)x[rs1] >> (x[rs2] & 0x1f));
}

void or(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("or %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	x[rd] = x[rs1] | x[rs2];
}

void and(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("and %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	x[rd] = x[rs1] & x[rs2];
}

void fence(void)
{
	printf("fence: unimplemented\n");
	exit(-1);
}

void fencei(void)
{
	printf("fencei: unimplemented\n");
	exit(-1);
}

void csrrw(void)
{
	printf("csrrw: unimplemented\n");
	exit(-1);
}

void csrrs(void)
{
	printf("csrrs: unimplemented\n");
	exit(-1);
}

void csrrc(void)
{
	printf("csrrc: unimplemented\n");
	exit(-1);
}

void csrrwi(void)
{
	printf("csrrwi: unimplemented\n");
	exit(-1);
}

void csrrsi(void)
{
	printf("csrrsi: unimplemented\n");
	exit(-1);
}

void csrrci(void)
{
	printf("csrrci: unimplemented\n");
	exit(-1);
}

void ecall(void)
{
	printf("ecall: unimplemented\n");
	exit(-1);
}

void ebreak(void)
{
	printf("ebreak: unimplemented\n");
	exit(-1);
}

void uret(void)
{
	printf("uret: unimplemented\n");
	exit(-1);
}

void sret(void)
{
	printf("sret: unimplemented\n");
	exit(-1);
}

void mret(void)
{
	printf("mret: unimplemented\n");
	exit(-1);
}

void wfi(void)
{
	printf("wfi: unimplemented\n");
	exit(-1);
}

void sfencevma(void)
{
	printf("sfencevma: unimplemented\n");
	exit(-1);
}

void lb(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t off = instr >> 20;
	off = sext(off, 12);

	if (PRINT) {
		printf("lb %s, %d(%s)\n", regs[rd], off, regs[rs1]);
	}

	x[rd] = sext(get_b(x[rs1] + off), 8);
}

void lh(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t off = instr >> 20;
	off = sext(off, 12);

	if (PRINT) {
		printf("lh %s, %d(%s)\n", regs[rd], off, regs[rs1]);
	}

	x[rd] = sext(get_hw(x[rs1] + off), 16);
}

void lw(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t off = instr >> 20;
	off = sext(off, 12);

	if (PRINT) {
		printf("lw %s, %d(%s)\n", regs[rd], off, regs[rs1]);
	}

	x[rd] = get_w(x[rs1] + off);
}

void lbu(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t off = instr >> 20;
	off = sext(off, 12);

	if (PRINT) {
		printf("lbu %s, %d(%s)\n", regs[rd], off, regs[rs1]);
	}

	x[rd] = (uint32_t)get_b(x[rs1] + off);
}

void lhu(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t off = instr >> 20;
	off = sext(off, 12);

	if (PRINT) {
		printf("lhu %s, %d(%s)\n", regs[rd], off, regs[rs1]);
	}

	x[rd] = (uint32_t)get_hw(x[rs1] + off);
}

void sb(void)
{
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;
	uint32_t off = (instr >> 25) << 5 | ((instr >> 7) & 0x1f);
	off = sext(off, 12);

	if (PRINT) {
		printf("sb %s, %d(%s)\n", regs[rs2], off, regs[rs1]);
	}

	set_b(x[rs1] + off, (uint8_t)(x[rs2] & 0xff));
}

void sh(void)
{
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;
	uint32_t off = (instr >> 25) << 5 | ((instr >> 7) & 0x1f);
	off = sext(off, 12);

	if (PRINT) {
		printf("sh %s, %d(%s)\n", regs[rs2], off, regs[rs1]);
	}

	set_hw(x[rs1] + off, (uint16_t)(x[rs2] & 0xffff));
}

void sw(void)
{
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;
	uint32_t off = (instr >> 25) << 5 | ((instr >> 7) & 0x1f);
	off = sext(off, 12);

	if (PRINT) {
		printf("sw %s, %d(%s)\n", regs[rs2], off, regs[rs1]);
	}

	set_w(x[rs1] + off, (uint32_t)x[rs2]);
}

void jal(void)
{
	uint32_t off1 = (instr >> 31) << 20; // 20
	uint32_t off2 = instr & 0xff000; // 19:12
	uint32_t off3 = ((instr >> 20) & 0x1) << 11; // 11
	uint32_t off4 = (instr >> 20) & 0x7fe; // 10:1

	size_t rd = (instr >> 7) & 0x1f;
	uint32_t off = off1 | off2 | off3 | off4;
	off = sext(off, 20);

	if (PRINT) {
		printf("jal %s, %x\n", regs[rd], off);
	}

	x[rd] = pc;
	pc += off - 4;
}

void jalr(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	uint32_t off = instr >> 20;
	off = sext(off, 12);

	if (PRINT) {
		printf("jalr %s, %s, %x\n", regs[rd], regs[rs1], off);
	}

	uint32_t t = pc;
	uint32_t new_pc = (x[rs1] + off) & ~1U;

	if (new_pc < text_begin || new_pc > text_end) {
		printf("EXCEPTION: Invalid memory address (out of range) %x\n", new_pc);
		return;
	}

	pc = new_pc;
	x[rd] = t;
}

void beq(void)
{
	uint32_t off1 = (instr >> 31) << 12; // 12
	uint32_t off2 = ((instr >> 25) & 0x3f) << 5; // 10:5
	uint32_t off3 = ((instr >> 8) & 0xf) << 1; // 4:1
	uint32_t off4 = ((instr >> 7) & 0x1) << 11; // 11

	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;
	uint32_t off = off1 | off2 | off3 | off4;
	off = sext(off, 12);

	if (PRINT) {
		printf("beq %s, %s, %x\n", regs[rs1], regs[rs2], off);
	}

	if (x[rs1] == x[rs2]) {
		pc += off - 4;
	}
}

void bne(void)
{
	uint32_t off1 = (instr >> 31) << 12; // 12
	uint32_t off2 = ((instr >> 25) & 0x3f) << 5; // 10:5
	uint32_t off3 = ((instr >> 8) & 0xf) << 1; // 4:1
	uint32_t off4 = ((instr >> 7) & 0x1) << 11; // 11

	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;
	uint32_t off = off1 | off2 | off3 | off4;
	off = sext(off, 12);

	if (PRINT) {
		printf("bne %s, %s, %x\n", regs[rs1], regs[rs2], off);
	}

	if (x[rs1] != x[rs2]) {
		pc += off - 4;
	}
}

void blt(void)
{
	uint32_t off1 = (instr >> 31) << 12; // 12
	uint32_t off2 = ((instr >> 25) & 0x3f) << 5; // 10:5
	uint32_t off3 = ((instr >> 8) & 0xf) << 1; // 4:1
	uint32_t off4 = ((instr >> 7) & 0x1) << 11; // 11

	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;
	uint32_t off = off1 | off2 | off3 | off4;
	off = sext(off, 12);

	if (PRINT) {
		printf("blt %s, %s, %x\n", regs[rs1], regs[rs2], off);
	}

	if ((int32_t)x[rs1] < (int32_t)x[rs2]) {
		pc += off - 4;
	}
}

void bge(void)
{
	uint32_t off1 = (instr >> 31) << 12; // 12
	uint32_t off2 = ((instr >> 25) & 0x3f) << 5; // 10:5
	uint32_t off3 = ((instr >> 8) & 0xf) << 1; // 4:1
	uint32_t off4 = ((instr >> 7) & 0x1) << 11; // 11

	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;
	uint32_t off = off1 | off2 | off3 | off4;
	off = sext(off, 12);

	if (PRINT) {
		printf("bge %s, %s, %x\n", regs[rs1], regs[rs2], off);
	}

	if ((int32_t)x[rs1] >= (int32_t)x[rs2]) {
		pc += off - 4;
	}
}

void bltu(void)
{
	uint32_t off1 = (instr >> 31) << 12; // 12
	uint32_t off2 = ((instr >> 25) & 0x3f) << 5; // 10:5
	uint32_t off3 = ((instr >> 8) & 0xf) << 1; // 4:1
	uint32_t off4 = ((instr >> 7) & 0x1) << 11; // 11

	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;
	uint32_t off = off1 | off2 | off3 | off4;
	off = sext(off, 12);

	if (PRINT) {
		printf("bltu %s, %s, %x\n", regs[rs1], regs[rs2], off);
	}

	if (x[rs1] < x[rs2]) {
		pc += off - 4;
	}
}

void bgeu(void)
{
	uint32_t off1 = (instr >> 31) << 12; // 12
	uint32_t off2 = ((instr >> 25) & 0x3f) << 5; // 10:5
	uint32_t off3 = ((instr >> 8) & 0xf) << 1; // 4:1
	uint32_t off4 = ((instr >> 7) & 0x1) << 11; // 11

	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;
	uint32_t off = off1 | off2 | off3 | off4;
	off = sext(off, 12);

	if (PRINT) {
		printf("bgeu %s, %s, %x\n", regs[rs1], regs[rs2], off);
	}

	if (x[rs1] >= x[rs2]) {
		pc += off - 4;
	}
}

void imm_arith_instr(void)
{
	uint32_t op2 = (instr >> 12) & 0x7;
	uint32_t op3 = (instr >> 27);

	switch (op2) {
		case 0b000:
			addi();
			break;
		case 0b010:
			slti();
			break;
		case 0b011:
			sltiu();
			break;
		case 0b100:
			xori();
			break;
		case 0b110:
			ori();
			break;
		case 0b111:
			andi();
			break;
		case 0b001:
			slli();
			break;
		case 0b101:
			switch (op3) {
				case 0b00000:
					srli();
					break;
				case 0b01000:
					srai();
					break;
				default:
					printf("imm_arith_instr(): srli/srai: invalid opcode %d\n", op3);
					exit(-1);
					break;
			}
			break;
		default:
			printf("imm_arith_instr(): invalid opcode %d\n", op2);
			exit(-1);
	}
}

void mul(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("mul %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	x[rd] = (uint32_t)((int64_t)(int32_t)x[rs1] * (int64_t)(int32_t)x[rs2]);
}

void mulh(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("mulh %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	x[rd] = (uint32_t)((uint64_t)((int64_t)(int32_t)x[rs1] * (int64_t)(int32_t)x[rs2]) >> 32);
}

void mulhsu(void)
{
	size_t rd = (instr >> 7) & 0x1F;
	size_t rs1 = (instr >> 15) & 0x1F;
	size_t rs2 = (instr >> 20) & 0x1F;

	if (PRINT) {
		printf("mulhsu %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	int64_t op1 = (int32_t)x[rs1];
	uint64_t op2 = (uint32_t)x[rs2];

	int64_t result = op1 * (int64_t)op2;
	uint32_t high_bits = (uint32_t)((uint64_t)result >> 32);

	if (rd != 0) {
		x[rd] = high_bits;
	}
}

void mulhu(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("mulhu %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	x[rd] = (uint32_t)((uint64_t)((uint64_t)x[rs1] * (uint64_t)x[rs2]) >> 32);
}

void _div(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("div %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	if (!x[rs2]) {
		printf("EXCEPTION: division by zero\n");
		x[rd] = (uint32_t)-1;
		return;
	}

	if(x[rs2] == (uint32_t)-1) {
		x[rd] = (uint32_t)(-((int32_t)x[rs1]));
		return;
	}

	x[rd] = (uint32_t)((int32_t)x[rs1] / (int32_t)x[rs2]);
}

void divu(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("divu %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	if (!x[rs2]) {
		printf("EXCEPTION: mod zero\n");
		x[rd] = (uint32_t)-1;
		return;
	}

	x[rd] = x[rs1] / x[rs2];
}

void rem(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (!x[rs2]) {
		x[rd] = x[rs1];
		return;
	}

	if (x[rs2] == (uint32_t)-1) {
		x[rd] = 0;
		return;
	}

	if (PRINT) {
		printf("rem %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	x[rd] = (uint32_t)((int32_t)x[rs1] % (int32_t)x[rs2]);
}

void remu(void)
{
	size_t rd = (instr >> 7) & 0x1f;
	size_t rs1 = (instr >> 15) & 0x1f;
	size_t rs2 = (instr >> 20) & 0x1f;

	if (PRINT) {
		printf("remu %s, %s, %s\n", regs[rd], regs[rs1], regs[rs2]);
	}

	if (!x[rs2]) {
		x[rd] = x[rs1];
		return;
	}

	x[rd] = x[rs1] % x[rs2];
}

void arith_instr(void)
{
	uint32_t op2 = (instr >> 12) & 0x7;
	uint32_t op3 = (instr >> 27);
	uint32_t op4 = (instr >> 25) & 0x3;

	switch (op4) {
		case 0b01:
			// rv32m
			switch (op2) {
				case 0b000:
					mul();
					break;
				case 0b001:
					mulh();
					break;
				case 0b010:
					mulhsu();
					break;
				case 0b011:
					mulhu();
					break;
				case 0b100:
					_div();
					break;
				case 0b101:
					divu();
					break;
				case 0b110:
					rem();
					break;
				case 0b111:
					remu();
					break;
				default:
					printf("arith_instr(): invalid op2 %d\n", op2);
					exit(-1);
					break;
			}
			break;

		case 0b00:
			// rv32i
			switch (op2) {
				case 0b000:
					switch (op3) {
						case 0b00000:
							add();
							break;
						case 0b01000:
							sub();
							break;
						default:
							printf("arith_instr(): 0b000 %d\n", op3);
							exit(-1);
							break;
					}
					break;
				case 0b001:
					sll();
					break;
				case 0b010:
					slt();
					break;
				case 0b011:
					sltu();
					break;
				case 0b100:
					xor();
					break;
				case 0b101:
					switch (op3) {
						case 0b00000:
							srl();
							break;
						case 0b01000:
							sra();
							break;
						default:
							printf("arith_instr(): 0b101 %d\n", op3);
							exit(-1);
							break;
					}
					break;
				case 0b110:
					or ();
					break;
				case 0b111:
					and();
					break;
				default:
					printf("arith_instr(): invalid op2 %d\n", op2);
					exit(-1);
					break;
			}
			break;

		default:
			printf("arith_instr(): invalid op4 %d\n", op4);
			exit(-1);
			break;
	}
}

void unimplemented(void)
{
	printf("unimplemented(): unimplemented instr\n");
	for (int i = 10; i < 15; i++) {
		if (rodata_begin <= x[i] && x[i] < rodata_end) {
			printf("x[%d] = %s\n", i, (char*)get_mem_addr(x[i]));
		}
	}

	print_regs();

	if(bss_begin == 0x60000000 && bss_end > 0x60000040)  {
		printf("test count: %d\n", ((uint8_t *)mem[BSS])[64]);
		for (uint8_t i = 0; i < 64; i++) {
			printf("test %d: %d\n", i + 1, ((uint8_t *)mem[BSS])[i]);
		}
	}

	exit(-1);
	// fence
	// fencei
	// csrrw
	// csrrs
	// csrrc
	// csrrwi
	// csrrsi
	// csrrci
	// ecall
	// ebreak
	// uret
	// sret
	// mret
	// wfi
	// sfencevma
}

void load(void)
{
	uint32_t op2 = (instr >> 12) & 0x7;

	switch (op2) {
		case 0b000:
			lb();
			break;
		case 0b001:
			lh();
			break;
		case 0b010:
			lw();
			break;
		case 0b100:
			lbu();
			break;
		case 0b101:
			lhu();
			break;
		default:
			printf("load(): invalid opcode %d\n", op2);
			exit(-1);
	}
}

void store(void)
{
	uint32_t op2 = (instr >> 12) & 0x7;

	switch (op2) {
		case 0b000:
			sb();
			break;
		case 0b001:
			sh();
			break;
		case 0b010:
			sw();
			break;
		default:
			printf("store(): invalid opcode %d\n", op2);
			exit(-1);
	}
}

void jump(void)
{
	uint32_t op2 = (instr >> 12) & 0x7;

	switch (op2) {
		case 0b000:
			beq();
			break;
		case 0b001:
			bne();
			break;
		case 0b100:
			blt();
			break;
		case 0b101:
			bge();
			break;
		case 0b110:
			bltu();
			break;
		case 0b111:
			bgeu();
			break;
		default:
			printf("jump(): invalid opcode %d\n", op2);
			exit(-1);
	}
}

void exec_instr(void)
{
	x[0] = 0;
	if (get_mem_seg(pc) != TEXT) {
		printf("exec_instr(): SEGFAULT\n");
		print_regs();
		exit(-1);
	}

	uint32_t *addr = (uint32_t *)(uint64_t)get_mem_addr(pc);
	instr = *addr;
	if (PRINT) {
		printf("brojac %ld\n", brojac);
		printf("%ld. pc = %08x\t\t sp = %08x\t\t a0 = %08x\t\t bytes = %08x\t\t ", ukupan_broj, pc, x[2], x[10], instr);
	}
	pc += 4;

	if ((instr & 0x3) != 0x3) {
		printf("Error: bits 1-0 not valid.\n");
		print_regs();
		exit(-1);
	}

	uint32_t op1 = (instr >> 2) & 0x1f;

	switch (op1) {
		case 0b01101:
			lui();
			break;
		case 0b00101:
			auipc();
			break;
		case 0b00100:
			imm_arith_instr();
			break;
		case 0b01100:
			arith_instr();
			break;
		case 0b00011:
		case 0b11100:
			unimplemented();
			break;
		case 0b00000:
			load();
			break;
		case 0b01000:
			store();
			break;
		case 0b11011:
			jal();
			break;
		case 0b11001:
			jalr();
			break;
		case 0b11000:
			jump();
			break;
		default:
			printf("exec_instr(): invalid opcode\n");
			exit(-1);
			break;
	}
	x[0] = 0;
	ukupan_broj++;
}

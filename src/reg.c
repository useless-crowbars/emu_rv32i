#include "instr.h"
#include "reg.h"

const char *regs[] = {
	"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
	"a6",	"a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6",
};

#include <stdio.h>

void print_regs()
{
	printf("%s \t %08x \t %d\n", regs[0], x[0], x[0]);
	printf("%s \t %08x \t %d\n", regs[1], x[1], x[1]);
	printf("%s \t %08x \t %d\n", regs[2], x[2], x[2]);
	printf("%s \t %08x \t %d\n", regs[3], x[3], x[3]);
	printf("%s \t %08x \t %d\n", regs[4], x[4], x[4]);
	printf("%s \t %08x \t %d\n", regs[5], x[5], x[5]);
	printf("%s \t %08x \t %d\n", regs[6], x[6], x[6]);
	printf("%s \t %08x \t %d\n", regs[7], x[7], x[7]);
	printf("%s \t %08x \t %d\n", regs[8], x[8], x[8]);
	printf("%s \t %08x \t %d\n", regs[9], x[9], x[9]);
	printf("%s \t %08x \t %d\n", regs[10], x[10], x[10]);
	printf("%s \t %08x \t %d\n", regs[11], x[11], x[11]);
	printf("%s \t %08x \t %d\n", regs[12], x[12], x[12]);
	printf("%s \t %08x \t %d\n", regs[13], x[13], x[13]);
	printf("%s \t %08x \t %d\n", regs[14], x[14], x[14]);
	printf("%s \t %08x \t %d\n", regs[15], x[15], x[15]);
	printf("%s \t %08x \t %d\n", regs[16], x[16], x[16]);
	printf("%s \t %08x \t %d\n", regs[17], x[17], x[17]);
	printf("%s \t %08x \t %d\n", regs[18], x[18], x[18]);
	printf("%s \t %08x \t %d\n", regs[19], x[19], x[19]);
	printf("%s \t %08x \t %d\n", regs[20], x[20], x[20]);
	printf("%s \t %08x \t %d\n", regs[21], x[21], x[21]);
	printf("%s \t %08x \t %d\n", regs[22], x[22], x[22]);
	printf("%s \t %08x \t %d\n", regs[23], x[23], x[23]);
	printf("%s \t %08x \t %d\n", regs[24], x[24], x[24]);
	printf("%s \t %08x \t %d\n", regs[25], x[25], x[25]);
	printf("%s \t %08x \t %d\n", regs[26], x[26], x[26]);
	printf("%s \t %08x \t %d\n", regs[27], x[27], x[27]);
	printf("%s \t %08x \t %d\n", regs[28], x[28], x[28]);
	printf("%s \t %08x \t %d\n", regs[29], x[29], x[29]);
	printf("%s \t %08x \t %d\n", regs[30], x[30], x[30]);
	printf("%s \t %08x \t %d\n", regs[31], x[31], x[31]);
}

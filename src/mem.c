#include "instr.h"
#include "reg.h"
#include "gpu.h"
#include "mem.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void *mem[9];

uint64_t* data_cnt = NULL;

SEG get_mem_seg(uint32_t address)
{
	if (TEXT_BEGIN <= address && address < TEXT_END) {
		return TEXT;
	}

	if (CRITICAL_BEGIN <= address && address < CRITICAL_END) {
		return CRITICAL;
	}

	if (DATA_BEGIN <= address && address < DATA_END) {
		return DATA;
	}

	if (BSS_BEGIN <= address && address < BSS_END) {
		return BSS;
	}

	if (RODATA_BEGIN <= address && address < RODATA_END) {
		return RODATA;
	}

	if (GPU_BEGIN <= address && address < GPU_END) {
		return GPU;
	}

	if (KB_BEGIN <= address && address < KB_END) {
		return KB;
	}

	if (STACK_BEGIN < address && address <= STACK_END) {
		return STACK;
	}

	if(address == BUFFER_SWITCH) {
		return REG;
	}

	printf("get_mem_seg(): SEGFAULT\n");
	print_regs();
	exit(-1);
}

uint64_t get_mem_addr(uint32_t address)
{
	if (data_cnt == NULL && STATS) {
		data_cnt = (uint64_t*) calloc(0x10000000, sizeof(uint64_t));
	}

	if (TEXT_BEGIN <= address && address < TEXT_END) {
		if(STATS) data_cnt[address - 0x10000000]++;
		return (uint64_t)mem[TEXT] + (address - TEXT_BEGIN);
	}

	if (CRITICAL_BEGIN <= address && address < CRITICAL_END) {
		return (uint64_t)mem[CRITICAL] + (address - CRITICAL_BEGIN);
	}

	if (DATA_BEGIN <= address && address < DATA_END) {
		return (uint64_t)mem[DATA] + (address - DATA_BEGIN);
	}

	if (BSS_BEGIN <= address && address < BSS_END) {
		return (uint64_t)mem[BSS] + (address - BSS_BEGIN);
	}

	if (RODATA_BEGIN <= address && address < RODATA_END) {
		return (uint64_t)mem[RODATA] + (address - RODATA_BEGIN);
	}

	if (GPU_BEGIN <= address && address < GPU_END) {
		return (uint64_t)mem[GPU] + (address - GPU_BEGIN);
	}

	if (KB_BEGIN <= address && address < KB_END) {
		return (uint64_t)&key_pressed;
	}

	if (STACK_BEGIN < address && address <= STACK_END) {
		return (uint64_t)mem[STACK] + (address - STACK_BEGIN);
	}

	if(address == BUFFER_SWITCH) {
		return (uint64_t)mem[REG];
	}

	printf("get_mem_addr(): SEGFAULT\naddress: %x\n", address);
	print_regs();
	exit(-1);
}

uint8_t get_b(uint32_t address)
{
	uint8_t *addr = (uint8_t *)get_mem_addr(address);
	return *addr;
}

uint16_t get_hw(uint32_t address)
{
	uint16_t *addr = (uint16_t *)get_mem_addr(address);
	return *addr;
}

uint32_t get_w(uint32_t address)
{
	uint32_t *addr = (uint32_t *)get_mem_addr(address);
	return *addr;
}

void set_b(uint32_t address, uint8_t val)
{
	SEG type = get_mem_seg(address);
	if (type == TEXT || type == RODATA) {
		printf("set_b(): SEGFAULT\n");
		print_regs();
		exit(-1);
	}

	uint8_t *addr = (uint8_t *)get_mem_addr(address);
	*addr = val;
}

void set_hw(uint32_t address, uint16_t val)
{
	SEG type = get_mem_seg(address);
	if (type == TEXT || type == RODATA) {
		printf("set_hw(): SEGFAULT\n");
		print_regs();
		exit(-1);
	}

	if (type == GPU) {
		printf("set_hw(): GPU access\n");
		exit(-1);
	}

	uint16_t *addr = (uint16_t *)get_mem_addr(address);
	*addr = val;
}

void set_w(uint32_t address, uint32_t val)
{
	SEG type = get_mem_seg(address);
	if (type == TEXT || type == RODATA) {
		printf("set_w(): SEGFAULT\n");
		print_regs();
		exit(-1);
	}

	if (type == GPU) {
		printf("set_w(): GPU access\n");
		exit(-1);
	}

	uint32_t *addr = (uint32_t *)get_mem_addr(address);
	*addr = val;
}

void print_stats() {
	FILE* f = fopen("stats", "w");
	for (int i = 0; i < 0x10000000; i++) {
		if(data_cnt[i] > 500000) {
			fprintf(f, "%x -> %lu\n", i + 0x10000000, data_cnt[i]);
		}
	}
	fclose(f);
}

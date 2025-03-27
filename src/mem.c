#include "reg.h"
#include "gpu.h"
#include "mem.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t text_begin = 0;
uint64_t text_end = 0;
uint64_t data_begin = 0;
uint64_t data_end = 0;
uint64_t bss_begin = 0;
uint64_t bss_end = 0;
uint64_t rodata_begin = 0;
uint64_t rodata_end = 0;

void *mem[9];

uint64_t* data_cnt = NULL;

SEG get_mem_seg(uint32_t address)
{
	if (text_begin <= address && address < text_end) {
		return TEXT;
	}

	if (CRITICAL_BEGIN <= address && address < CRITICAL_END) {
		return CRITICAL;
	}

	if (data_begin <= address && address < data_end) {
		return DATA;
	}

	if (bss_begin <= address && address < bss_end) {
		return BSS;
	}

	if (rodata_begin <= address && address < rodata_end) {
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

	if (text_begin <= address && address < text_end) {
		if(STATS) data_cnt[address - 0x10000000]++;
		return (uint64_t)mem[TEXT] + (address - text_begin);
	}

	if (CRITICAL_BEGIN <= address && address < CRITICAL_END) {
		if(STATS) data_cnt[address - 0x10000000]++;
		return (uint64_t)mem[CRITICAL] + (address - CRITICAL_BEGIN);
	}

	if (data_begin <= address && address < data_end) {
		if(STATS) data_cnt[address - 0x10000000]++;
		return (uint64_t)mem[DATA] + (address - data_begin);
	}

	if (bss_begin <= address && address < bss_end) {
		if(STATS) data_cnt[address - 0x10000000]++;
		return (uint64_t)mem[BSS] + (address - bss_begin);
	}

	if (rodata_begin <= address && address < rodata_end) {
		return (uint64_t)mem[RODATA] + (address - rodata_begin);
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

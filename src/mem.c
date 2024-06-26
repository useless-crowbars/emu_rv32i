#include "instr.h"
#include "reg.h"
#include "gpu.h"
#include "mem.h"

#include <stdio.h>
#include <stdlib.h>

void *mem[6];

SEG get_mem_seg(uint32_t address)
{
	if (address < TEXT_END) {
		return TEXT;
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

	if (STACK_BEGIN <= address) {
		return STACK;
	}

	printf("get_mem_seg(): SEGFAULT\n");
	print_regs();
	exit(-1);
}

uint64_t get_mem_addr(uint32_t address)
{
	if (address < TEXT_END) {
		return (uint64_t)mem[TEXT] + (address - TEXT_BEGIN);
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

	if (STACK_BEGIN <= address) {
		return (uint64_t)mem[STACK] + (address - STACK_BEGIN);
	}

	printf("get_mem_addr(): SEGFAULT");
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

	if (type == GPU) {
		printf("set_b(): GPU access\n");
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
		uint32_t color = 0xff000000;
		color |= (val & 0x0f00) << 12;
		color |= (val & 0x00f0) << 8;
		color |= (val & 0x000f) << 4;

		uint32_t off = (address - GPU_BEGIN) / 2;
		uint32_t x = off % SCREEN_WIDTH;
		uint32_t y = off / SCREEN_WIDTH;

		write_screen(x, y, color);
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

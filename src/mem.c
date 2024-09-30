#include "instr.h"
#include "reg.h"
#include "gpu.h"
#include "mem.h"

#include <stdio.h>
#include <stdlib.h>

void *mem[7];

SEG get_mem_seg(uint32_t address)
{
	if (TEXT_BEGIN <= address && address < TEXT_END) {
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

	if (KB_BEGIN <= address && address < KB_END) {
		return KB;
	}

	if (STACK_BEGIN <= address && address <= STACK_END) {
		return STACK;
	}

	printf("get_mem_seg(): SEGFAULT\n");
	print_regs();
	exit(-1);
}

uint64_t get_mem_addr(uint32_t address)
{
	if (TEXT_BEGIN <= address && address < TEXT_END) {
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

	if (KB_BEGIN <= address && address < KB_END) {
		return (uint64_t)&key_pressed;
	}

	if (STACK_BEGIN <= address && address < STACK_END) {
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
		uint32_t color = 0xff1f1f1f;
		// BGR (2,3,3)
		color |= (val & 0x07) << 5;
		color |= (val & 0x38) << (13 - 3);
		color |= (val & 0xc0) << (22 - 6);

		uint32_t off = (address - GPU_BEGIN);

		write_screen((int)(off % SCREEN_WIDTH), (int)(off / SCREEN_WIDTH), color);
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

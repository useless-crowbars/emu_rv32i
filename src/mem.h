#ifndef MEM_H
#define MEM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define TEXT_BEGIN 0x00000000
#define TEXT_END 0x00030000
#define DATA_BEGIN 0x00100000
#define DATA_END 0x00110000
#define BSS_BEGIN 0x00300000
#define BSS_END 0x00400000
#define RODATA_BEGIN 0x10000000
#define RODATA_END 0x10300000
#define GPU_BEGIN 0x20000000
#define GPU_END 0x20020000
#define STACK_BEGIN 0xfff00000
#define STACK_END 0x00000000

typedef enum { TEXT, DATA, BSS, RODATA, GPU, STACK } SEG;

extern void *mem[6];

SEG get_mem_seg(uint32_t address);
uint64_t get_mem_addr(uint32_t address);
uint8_t get_b(uint32_t address);
uint16_t get_hw(uint32_t address);
uint32_t get_w(uint32_t address);
void set_b(uint32_t address, uint8_t val);
void set_hw(uint32_t address, uint16_t val);
void set_w(uint32_t address, uint32_t val);

#endif

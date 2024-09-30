#ifndef MEM_H
#define MEM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define TEXT_BEGIN 0x10000000
#define TEXT_END 0x10040000
#define DATA_BEGIN 0x10040000
#define DATA_END 0x10050000
#define BSS_BEGIN 0x10050000
#define BSS_END 0x10150000
#define RODATA_BEGIN 0x10150000
#define RODATA_END 0x10400000

#define GPU_BEGIN 0x20000000
#define GPU_END 0x20010000
#define KB_BEGIN 0x30000000
#define KB_END 0x30000001
#define STACK_BEGIN 0x00007000
#define STACK_END 0x00008000

typedef enum { TEXT, DATA, BSS, RODATA, GPU, KB, STACK } SEG;

extern void *mem[7];

SEG get_mem_seg(uint32_t address);
uint64_t get_mem_addr(uint32_t address);
uint8_t get_b(uint32_t address);
uint16_t get_hw(uint32_t address);
uint32_t get_w(uint32_t address);
void set_b(uint32_t address, uint8_t val);
void set_hw(uint32_t address, uint16_t val);
void set_w(uint32_t address, uint32_t val);

#endif

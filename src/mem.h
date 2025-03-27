#ifndef MEM_H
#define MEM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

extern uint64_t text_begin;
extern uint64_t text_end;
extern uint64_t data_begin;
extern uint64_t data_end;
extern uint64_t bss_begin;
extern uint64_t bss_end;
extern uint64_t rodata_begin;
extern uint64_t rodata_end;

#define STACK_BEGIN 0x00000000
#define STACK_END 0x00003000
#define CRITICAL_BEGIN 0x10031c00
#define CRITICAL_END 0x10032000
#define GPU_BEGIN 0x20000000
#define GPU_END 0x20010000
#define KB_BEGIN 0x30000000
#define KB_END 0x30000001
#define BUFFER_SWITCH 0x50000008

#define STATS 0

typedef enum { TEXT, DATA, BSS, RODATA, GPU, KB, STACK, REG, CRITICAL } SEG;

extern void *mem[9];

SEG get_mem_seg(uint32_t address);
uint64_t get_mem_addr(uint32_t address);
uint8_t get_b(uint32_t address);
uint16_t get_hw(uint32_t address);
uint32_t get_w(uint32_t address);
void set_b(uint32_t address, uint8_t val);
void set_hw(uint32_t address, uint16_t val);
void set_w(uint32_t address, uint32_t val);

void print_stats(void);

#endif

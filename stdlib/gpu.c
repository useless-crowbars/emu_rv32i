#include "gpu.h"

#ifdef __riscv
uint8_t* key = (uint8_t)0x30000000;

#define GPU_ADDR 0x20000000

void init_window()
{
}

void init_screen()
{
}

__attribute__((section(".critical")))
void write_screen(uint32_t x, uint32_t y, uint32_t color)
{
	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
		__asm__ __volatile__("ecall;");
	}

	uint8_t *gpu = (uint8_t *)GPU_ADDR;
	uint8_t color8 = 0;
	// BGR (2,3,3)
	color8 |= (0x000000e0 & color) >> 5;
	color8 |= (0x0000e000 & color) >> (13 - 3);
	color8 |= (0x00c00000 & color) >> (22 - 6);

	gpu[y * SCREEN_WIDTH + x] = color8;
}

void update_screen()
{
}

void poll_keyboard()
{
}

void cleanup()
{
}

#endif

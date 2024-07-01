#include "gpu.h"

#ifdef __riscv
bool* end = (bool*)0x30000000;
char* key_changed = (char*)0x30000008;
bool* key_pressed = (bool*)0x30000010;
bool* change_ack = (bool*)0x30000018;

#define GPU_ADDR 0x20000000

void init_window()
{
}

void init_screen()
{
}

void write_screen(uint32_t x, uint32_t y, uint32_t color)
{
	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
		__asm__ __volatile__("ecall;");
	}

	uint16_t *gpu = (uint16_t *)GPU_ADDR;
	uint16_t color16 = 0;
	color16 |= (0x000000f0 & color) >> 4;
	color16 |= (0x0000f000 & color) >> 8;
	color16 |= (0x00f00000 & color) >> 12;
	color16 |= (0xf0000000 & color) >> 16;

	gpu[y * SCREEN_WIDTH + x] = color16;
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

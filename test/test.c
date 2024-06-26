#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../stdlib/gpu.h"
#include "../stdlib/my_stdlib.h"
#include "../stdlib/riscv32im.h"

int main()
{
	uint16_t *gpu = (uint16_t *)0x20000000;

	volatile size_t a = SCREEN_WIDTH;
	for (uint32_t y = 0; y < SCREEN_HEIGHT; y++) {
		for (uint32_t x = 0; x < SCREEN_WIDTH; x++) {
			size_t lmao = y * a + x;
			gpu[lmao] = 0xff0f; // RGB
		}
	}
	return 0;
}

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../stdlib/gpu.h"
#include "../stdlib/my_stdlib.h"

int main()
{
	uint16_t *gpu = (uint16_t *)0x20000000;

	for (uint32_t y = 0; y < SCREEN_HEIGHT; y++) {
		for (uint32_t x = 0; x < SCREEN_WIDTH; x++) {
			if ((x + y) % 10 == 0) {
				write_screen(x, y, 0xffffffff);
			} else {
				write_screen(x, y, 0xff000000);
			}
		}
	}
	return 0;
}

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
			if ((x - 100) * (x - 100) + (y - 80) * (y - 80) < 10 * 10) {
				write_screen(x, y, 0xff6a33aa); // BGR
				continue;
			}

			if ((x - 100) * (x - 100) + (y - 100) * (y - 100) < 50 * 50) {
				write_screen(x, y, 0xff69ace0); // BGR
			}

			if ((x - 200) * (x - 200) + (y - 80) * (y - 80) < 10 * 10) {
				write_screen(x, y, 0xff6a33aa); // BGR
				continue;
			}

			if ((x - 200) * (x - 200) + (y - 100) * (y - 100) < 50 * 50) {
				write_screen(x, y, 0xff69ace0); // BGR
			}
		}
	}
	return 0;
}

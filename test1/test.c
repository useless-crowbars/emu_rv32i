#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../stdlib/gpu.h"
#include "../stdlib/my_stdlib.h"

int main()
{
	for (uint32_t y = 0; y < SCREEN_HEIGHT; y++) {
		for (uint32_t x = 0; x < SCREEN_WIDTH; x++) {
			if ((x - 50) * (x - 50) + (y - 60) * (y - 60) < 5 * 5) {
				write_screen(x, y, 0xff6a33aa); // BGR
				continue;
			}

			if ((x - 50) * (x - 50) + (y - 50) * (y - 50) < 25 * 25) {
				write_screen(x, y, 0xff69ace0); // BGR
			}

			if ((x - 100) * (x - 100) + (y - 60) * (y - 60) < 5 * 5) {
				write_screen(x, y, 0xff6a33aa); // BGR
				continue;
			}

			if ((x - 100) * (x - 100) + (y - 50) * (y - 50) < 25 * 25) {
				write_screen(x, y, 0xff69ace0); // BGR
			}
		}
	}

	while(1) {
		__asm__ __volatile__ ("nop");
	}

	return 0;
}

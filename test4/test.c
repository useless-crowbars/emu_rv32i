#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define GPU_ADDR 0x20000000UL
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 100

int main() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int wave_height = 20;
            int wave_position = (x / wave_height) % 2;

            if (wave_position == 1 && (y > 30 && y < 70)) {
				(*(uint8_t*)(GPU_ADDR + y * SCREEN_WIDTH + x)) = 0b00000111;
            } else {
				(*(uint8_t*)(GPU_ADDR + y * SCREEN_WIDTH + x)) = 0b00000000;
            }
        }
    }

    while(true) {
    	    __asm__ __volatile__ ("nop");
    }

	return 0;
}

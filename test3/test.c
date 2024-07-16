#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define GPU_ADDR 0x20000000UL

int main()
{
	uint8_t j = 0;
	while (1) {
		for (uint32_t i = 0; i < 160 * 100; i++) {
			(*(uint8_t*)(GPU_ADDR + i)) = j;
			j++;
		}
	}
	return 0;
}

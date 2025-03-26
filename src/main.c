#include "instr.h"
#include "elf.h"
#include "mem.h"
#include "gpu.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <elf_file> \n", argv[0]);
        return EXIT_FAILURE;
    }

	load_elf(argv[1]);
	init_screen();

	const int32_t update_interval = 300000;
	int32_t i = 0;

	uint64_t limit = INT_MAX;
	while(!STATS || limit) {
		/*
		int bri;
		scanf("%d", &bri);

		if (bri > 0) {
			for (int i = 0; i < bri; i++) {
				exec_instr();
			}
		} else if (bri == -1){
			print_regs();
		} else if (bri == -2) {
			uint32_t addr;
			scanf("%x", &addr);

			uint32_t* real_addr = (uint32_t*)get_mem_addr(addr);
			printf("M[%x] = %d / %x\n", (uint32_t)addr, *real_addr, *real_addr);
		}
		*/
		exec_instr();
		if (i++ >= update_interval) {
			update_screen();
			i = 0;
		}
		limit--;
	}
	print_stats();
}

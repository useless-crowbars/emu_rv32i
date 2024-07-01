#include "instr.h"
#include "mem.h"
#include "gpu.h"
#include "reg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

size_t open_file(const char *filename, void **ptr);
void init_mem(void);
int main(void);

size_t open_file(const char *filename, void **ptr)
{
	int text_fd;
	struct stat st;
	size_t len_file;

	text_fd = open(filename, O_RDONLY);
	if (text_fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (fstat(text_fd, &st) == -1) {
		perror("fstat");
		close(text_fd);
		exit(EXIT_FAILURE);
	}
	len_file = (size_t)st.st_size;

	*ptr = mmap(NULL, len_file, PROT_READ, MAP_SHARED, text_fd, 0);
	if (*ptr == MAP_FAILED) {
		perror("mmap");
		close(text_fd);
		exit(EXIT_FAILURE);
	}

	return len_file;
}

void init_mem()
{
	mem[GPU] = malloc(0x20000); // 128kB
	mem[STACK] = malloc(0x100000); // 1MB

	open_file("./text.bin", &mem[TEXT]);
	mem[DATA] = malloc(0x10000); // 64kB
	if (mem[DATA] == NULL) {
		printf("mem[DATA] == NULL\n");
		exit(-1);
	}
	mem[BSS] = calloc(0x100000, sizeof(uint8_t)); // 1MB
	if (mem[BSS] == NULL) {
		printf("mem[BSS] == NULL\n");
		exit(-1);
	}
	open_file("rodata.bin", &mem[RODATA]);

	uint8_t *data_copy;
	size_t data_len = open_file("data.bin", (void **)&data_copy);
	memcpy(mem[DATA], data_copy, data_len);
}

int main()
{
	init_mem();
	init_screen();

	const int32_t update_interval = 300000;
	int32_t i = 0;

	while (true) {
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
	}
}

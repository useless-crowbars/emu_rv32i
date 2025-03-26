#include "elf.h"
#include "mem.h"
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <gelf.h>

typedef struct {
	uint64_t original_addr;
	uint64_t size;
	void* data;
} SectionPart;

void extract_section(const char* elf_file, SEG segment) {
	const char *section_name;
	void **dest;

	switch(segment) {
		case TEXT:
			section_name = ".text";
			dest = &mem[TEXT];
			break;
		case DATA:
			section_name = ".data";
			dest = &mem[DATA];
			break;
		case RODATA:
			section_name = ".rodata";
			dest = &mem[RODATA];
			break;
		default:
			printf("Unsupported segment type");
			exit(EXIT_FAILURE);
	}

	if (elf_version(EV_CURRENT) == EV_NONE) {
		printf("ELF library initialization failed: %s", elf_errmsg(-1));
		exit(EXIT_FAILURE);
	}

	int fd = open(elf_file, O_RDONLY, 0);
	if (fd < 0) {
		printf("open '%s' failed", elf_file);
		exit(EXIT_FAILURE);
	}

	Elf* elf = elf_begin(fd, ELF_C_READ, NULL);
	if (!elf) {
		close(fd);
		printf("elf_begin() failed: %s", elf_errmsg(-1));
		exit(EXIT_FAILURE);
	}

	Elf_Scn* scn = NULL;
	GElf_Shdr shdr;
	size_t shstrndx;
	SectionPart* sections = NULL;
	size_t num_sections = 0;
	uint64_t min_addr = UINT64_MAX;
	uint64_t max_addr = 0;

	if (elf_getshdrstrndx(elf, &shstrndx) != 0) {
		elf_end(elf);
		close(fd);
		printf("elf_getshdrstrndx() failed: %s", elf_errmsg(-1));
		exit(EXIT_FAILURE);
	}

	while ((scn = elf_nextscn(elf, scn))) {
		if (gelf_getshdr(scn, &shdr) != &shdr) {
			continue;
		}

		char* name = elf_strptr(elf, shstrndx, shdr.sh_name);
		if (!name || strcmp(name, section_name) != 0) {
			continue;
		}

		Elf_Data* data = elf_getdata(scn, NULL);
		if (!data || !data->d_buf || shdr.sh_size == 0) {
			continue;
		}

		SectionPart* new_sections = realloc(sections, (num_sections + 1) * sizeof(SectionPart));
		if (!new_sections) {
			free(sections);
			elf_end(elf);
			close(fd);
			printf("memory allocation failed");
			exit(EXIT_FAILURE);
		}
		sections = new_sections;

		sections[num_sections].original_addr = shdr.sh_addr;
		sections[num_sections].size = shdr.sh_size;
		sections[num_sections].data = data->d_buf;
		num_sections++;

		if (shdr.sh_addr < min_addr) {
			min_addr = shdr.sh_addr;
		}
		if (shdr.sh_addr + shdr.sh_size > max_addr) {
			max_addr = shdr.sh_addr + shdr.sh_size;
		}
	}

	if (num_sections == 0) {
		free(sections);
		elf_end(elf);
		close(fd);
		printf("section %s not found in %s", section_name, elf_file);
		return;
	}

	if (*dest == NULL) {
		*dest = calloc(1, max_addr - min_addr);
		if (!*dest) {
			free(sections);
			elf_end(elf);
			close(fd);
			exit(EXIT_FAILURE);
		}
	}

	for (size_t i = 0; i < num_sections; i++) {
		uint64_t offset = (segment == TEXT) ? (sections[i].original_addr - min_addr) : 0;
		if (offset + sections[i].size > max_addr - min_addr) {
			printf("section %s overflow in %s", section_name, elf_file);
			continue;
		}
		memcpy((uint8_t*)*dest + offset, sections[i].data, sections[i].size);
	}

	free(sections);
	elf_end(elf);
	close(fd);
}

void load_elf(const char* elf_file) {
	mem[CRITICAL] = calloc(0x800, 1);
	mem[GPU] = malloc(0x8001);
	mem[STACK] = malloc(0x1000);
	mem[REG] = malloc(1);

	mem[DATA] = malloc(0x10000);
	mem[BSS] = calloc(0x100000, 1);

	extract_section(elf_file, TEXT);
	extract_section(elf_file, DATA);
	extract_section(elf_file, RODATA);
}

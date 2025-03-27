#include "elf.h"
#include "instr.h"
#include "mem.h"
#include <elf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <gelf.h>

#define MAX_SECTIONS 50

typedef struct {
	uint64_t original_addr;
	uint64_t size;
	void* data;
} SectionPart;

typedef struct {
	uint64_t min_addr;
	uint64_t max_addr;
	uint64_t total_size;
	int count;
} SectionInfo;

typedef struct {
    Elf* elf;
    int fd;
} ElfHandle;

ElfHandle open_elf(const char* elf_file);
void close_elf(ElfHandle handle);
void init_section_bounds(ElfHandle handle);
void init_pc(ElfHandle handle);
void extract_section(ElfHandle handle, SEG segment);

ElfHandle open_elf(const char* elf_file) {
    ElfHandle handle = {NULL, -1};
    
    if (elf_version(EV_CURRENT) == EV_NONE) {
        printf("ELF library initialization failed: %s", elf_errmsg(-1));
        exit(EXIT_FAILURE);
    }

    handle.fd = open(elf_file, O_RDONLY, 0);
    if (handle.fd < 0) {
        printf("open '%s' failed", elf_file);
        exit(EXIT_FAILURE);
    }

    handle.elf = elf_begin(handle.fd, ELF_C_READ, NULL);
    if (!handle.elf) {
        close(handle.fd);
        printf("elf_begin() failed: %s", elf_errmsg(-1));
        exit(EXIT_FAILURE);
    }

    return handle;
}

void close_elf(ElfHandle handle) {
    if (handle.elf) {
        elf_end(handle.elf);
    }
    if (handle.fd >= 0) {
        close(handle.fd);
    }
}

void init_section_bounds(ElfHandle handle) {
    Elf_Scn* scn = NULL;
    GElf_Shdr shdr;
    size_t shstrndx;

    if (elf_getshdrstrndx(handle.elf, &shstrndx) != 0) {
        printf("elf_getshdrstrndx() failed: %s", elf_errmsg(-1));
        return;
    }

    while ((scn = elf_nextscn(handle.elf, scn))) {
        if (gelf_getshdr(scn, &shdr) != &shdr) {
            continue;
        }

        char* name = elf_strptr(handle.elf, shstrndx, shdr.sh_name);
        if (!name || shdr.sh_size == 0) {
            continue;
        }

        if (strcmp(name, ".text") == 0) {
            if (text_begin == 0 || shdr.sh_addr < text_begin)
                text_begin = shdr.sh_addr;
            if (shdr.sh_addr + shdr.sh_size > text_end)
                text_end = shdr.sh_addr + shdr.sh_size;
        }
        else if (strcmp(name, ".data") == 0) {
            if (data_begin == 0 || shdr.sh_addr < data_begin)
                data_begin = shdr.sh_addr;
            if (shdr.sh_addr + shdr.sh_size > data_end)
                data_end = shdr.sh_addr + shdr.sh_size;
        }
        else if (strcmp(name, ".bss") == 0) {
            if (bss_begin == 0 || shdr.sh_addr < bss_begin)
                bss_begin = shdr.sh_addr;
            if (shdr.sh_addr + shdr.sh_size > bss_end)
                bss_end = shdr.sh_addr + shdr.sh_size;
        }
        else if (strcmp(name, ".rodata") == 0) {
            if (rodata_begin == 0 || shdr.sh_addr < rodata_begin)
                rodata_begin = shdr.sh_addr;
            if (shdr.sh_addr + shdr.sh_size > rodata_end)
                rodata_end = shdr.sh_addr + shdr.sh_size;
        }
    }

    printf(".text:   0x%lx - 0x%lx\n", text_begin, text_end);
    printf(".data:   0x%lx - 0x%lx\n", data_begin, data_end);
    printf(".bss:    0x%lx - 0x%lx\n", bss_begin, bss_end);
    printf(".rodata: 0x%lx - 0x%lx\n", rodata_begin, rodata_end);
}

void init_pc(ElfHandle handle) {
    uint64_t entry_point = 0x0;

    GElf_Ehdr ehdr;
    if (gelf_getehdr(handle.elf, &ehdr) != NULL) {
        entry_point = ehdr.e_entry;
        printf("ELF entry point found: 0x%lx\n", entry_point);
    } else {
        printf("Warning: Using default PC value 0x%x\n", (uint32_t)0x0);
    }

    if (text_begin != 0 && text_end != 0) {
        if (entry_point < text_begin || entry_point >= text_end) {
            printf("Warning: Entry point 0x%lx outside .text section (0x%lx-0x%lx)\n",
                   entry_point, text_begin, text_end);
            entry_point = text_begin;
        }
    }

    pc = (uint32_t)entry_point;
}

void extract_section(ElfHandle handle, SEG segment) {
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
			printf("Unsupported segment type\n");
			exit(EXIT_FAILURE);
	}

	Elf_Scn* scn = NULL;
	GElf_Shdr shdr;
	size_t shstrndx;
	SectionPart* sections = NULL;
	size_t num_sections = 0;
	uint64_t min_addr = UINT64_MAX;
	uint64_t max_addr = 0;

	if (elf_getshdrstrndx(handle.elf, &shstrndx) != 0) {
		printf("elf_getshdrstrndx() failed: %s\n", elf_errmsg(-1));
		exit(EXIT_FAILURE);
	}

	while ((scn = elf_nextscn(handle.elf, scn))) {
		if (gelf_getshdr(scn, &shdr) != &shdr) {
			continue;
		}

		char* name = elf_strptr(handle.elf, shstrndx, shdr.sh_name);
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
			printf("memory allocation failed\n");
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
		printf("Section %s not found\n", section_name);
		return;
	}

	if (*dest == NULL) {
		*dest = calloc(1, max_addr - min_addr);
		if (!*dest) {
			free(sections);
			exit(EXIT_FAILURE);
		}
	}

	for (size_t i = 0; i < num_sections; i++) {
		uint64_t offset = (segment == TEXT) ? (sections[i].original_addr - min_addr) : 0;
		if (offset + sections[i].size > max_addr - min_addr) {
			printf("section %s overflow\n", section_name);
			continue;
		}
		memcpy((uint8_t*)*dest + offset, sections[i].data, sections[i].size);
	}

	free(sections);
}

void load_elf(const char* elf_file) {
	ElfHandle handle = open_elf(elf_file);

	init_section_bounds(handle);
	init_pc(handle);

	mem[CRITICAL] = calloc(0x800, 1);
	mem[GPU] = malloc(0x8001);
	mem[STACK] = malloc(0x1000);
	mem[REG] = malloc(1);

	mem[DATA] = malloc(data_end - data_begin);
	mem[BSS] = calloc(bss_end - bss_begin, 1);

	extract_section(handle, TEXT);
	extract_section(handle, DATA);
	extract_section(handle, RODATA);

	close_elf(handle);
}

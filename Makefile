.PHONY: all clean

all:
	cp -f ../doom_rv32i/src/emdoom .
	riscv64-elf-objcopy -O binary emdoom text.bin -j.text
	riscv64-elf-objcopy -O binary emdoom data.bin -j.data
	riscv64-elf-objcopy -O binary emdoom rodata.bin -j.rodata
	riscv64-elf-objdump -D emdoom -j.text | grep ">:" > file
	# gcc -O3 src/*.c -o emu_rv32i -lglfw -lGL && ./emu_rv32i
	gcc -O3 -Wno-unused-parameter -Wall -Werror -Wextra -Wshadow -Wcast-align -Wpointer-arith -Wmissing-prototypes -Wmissing-declarations -Wwrite-strings -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Wconversion -Wstrict-prototypes src/*.c -o emu_rv32i -lglfw -lGL && ./emu_rv32i

clean:
	rm -rf emdoom text.bin data.bin bss.bin rodata.bin emu_rv32i file

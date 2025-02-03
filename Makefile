.PHONY: all clean

all: ../doom_rv32i/src/emdoom
	riscv64-elf-objcopy -O binary ../doom_rv32i/src/emdoom text.bin -j.text
	riscv64-elf-objcopy -O binary ../doom_rv32i/src/emdoom data.bin -j.data
	riscv64-elf-objcopy -O binary ../doom_rv32i/src/emdoom rodata.bin -j.rodata
	riscv64-elf-objdump -D ../doom_rv32i/src/emdoom -j.text | grep ">:" > file
	# gcc -O3 src/*.c -o emu_rv32i -lglfw -lGL && ./emu_rv32i
	gcc -O3 -Wno-unused-parameter -Wall -Werror -Wextra -Wshadow -Wcast-align -Wpointer-arith -Wmissing-prototypes -Wmissing-declarations -Wwrite-strings -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Wconversion -Wstrict-prototypes src/*.c -o emu_rv32i -lglfw -lGL

run:
	./emu_rv32i

clean:
	rm -rf text.bin data.bin bss.bin rodata.bin emu_rv32i file stats

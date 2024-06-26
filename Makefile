.PHONY: all clean

all:
	cp -f ../doom_rv32i/src/emdoom .
	riscv64-elf-objcopy -O binary emdoom text.bin -j.text
	riscv64-elf-objcopy -O binary emdoom data.bin -j.data
	riscv64-elf-objcopy -O binary emdoom bss.bin -j.bss
	riscv64-elf-objcopy -O binary emdoom rodata.bin -j.rodata
	gcc src/*.c -o emu_rv32i -lglfw -lGL && ./emu_rv32i

clean:
	rm -rf text.bin data.bin bss.bin rodata.bin emu_rv32i

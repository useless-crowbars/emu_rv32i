CFLAGS:=-Os -D__STDC_HOSTED__=0 -w $(IGNORE_WARNINGS) -march=rv32im -mabi=ilp32 -mstrict-align -nostdlib -ffreestanding -fdata-sections -ffunction-sections
LDFLAGS:= -T linker.ld $(CFLAGS)
SUPPORT_FILES:=startup.S ../stdlib/*.c
CC:=riscv64-elf-gcc

all:
	$(CC) $(CFLAGS) test.c $(SUPPORT_FILES) -o test $(LDFLAGS)
	riscv64-elf-objcopy -O binary test text.bin -j.text

clean:
	rm -f ./test ./text.bin

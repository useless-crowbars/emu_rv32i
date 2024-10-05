OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
/*OUTPUT_ARCH(rv32i)*/

ENTRY(_start)

SECTIONS
{
    . = 0x10000000;
    .text :
    {
	*(.text)
	*(.text.*)
    }

    . = 0x10040000;
    .data :
    {
	*(.data)
	*(.data.*)
	*(.sdata)
	*(.sdata.*)
    }

    . = 0x10050000;
    .bss :
    {
	*(.bss)
	*(.bss.*)
	*(.sbss)
	*(.sbss.*)
    }

    . = 0x10150000;
    .rodata :
    {
        *(.rodata)
        *(.rodata.*)
        *(.srodata)
        *(.srodata.*)
    }

    . = 0x10400000;

    /DISCARD/ :
    {
        *(.debug_info)
        *(.debug_abbrev)
        *(.debug_aranges)
        *(.debug_line)
        *(.debug_str)
        *(.debug_line_str)
        *(.debug_frame)
        *(.comment)
        *(.riscv.attributes)
        *(.symtab)
        *(.strtab)
        *(.shstrtab)
    }
}

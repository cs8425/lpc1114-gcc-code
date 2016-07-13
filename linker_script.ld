/* useful reference: www.linuxselfhelp.com/gnu/ld/html_chapter/ld_toc.html */
/* Written by Frank Duignan */
MEMORY
{
	flash : org = 0x00000000, len = 32k
	ram : org = 0x10000000, len = 0x1F40 /* 8k */
}

__top_RamLoc8 = 0x10000000 + 0x1F40;
  
SECTIONS
{

	. = ORIGIN(flash);
	.text : {
		*(.vectors); /* The interrupt vectors */
		*(.text);
		*(.rodata .rodata.*);
	} > flash

	. = ORIGIN(ram);
	.data : {
		INIT_DATA_VALUES = LOADADDR(.data);
		INIT_DATA_START = .;
		*(.data .data.*);
		INIT_DATA_END = .;
	} > ram AT>flash

	BSS_START = .;
	.bss : {

		*(.bss .bss.*);

	} > ram
	BSS_END = .;

	PROVIDE(_pvHeapStart = .);
	PROVIDE(_vStackTop = __top_RamLoc8 - 0);
}

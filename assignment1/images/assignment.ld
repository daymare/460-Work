

ENTRY(start)

SECTIONS
{
    . = 0x10000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }
    .data : { font.o }
    .data : { image.o }
    . =ALIGN(8);
    . =. + 0x1000;
    stack_top = .;
}

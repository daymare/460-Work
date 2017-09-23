/****************
qemu with -kernel file.bin ==> start to execute from 0x10000 ==>
file.bin must be loaded to 0x10000 ==> . = 0x10000
************/
ENTRY(vectors_start)
SECTIONS
{
  . = 0x10000;
  .text : { *(text) }
  .data : { *(.data) }
  .bss  : { *(.bss) }

  .data : { font0.o }
  
  . = ALIGN(8);
  . = . + 0x1000; /* 4kB of irq stack memory */
  irq_stack = .;
}


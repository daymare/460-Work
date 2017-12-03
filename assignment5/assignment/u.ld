/****************
qemu with -kernel file.bin ==> start to execute from 0x10000 ==>
file.bin must be loaded to 0x10000 ==> . = 0x10000
************/
ENTRY(vectors_start)
SECTIONS
{
  . = 0x10000; /* loading address */
  .text : { us.o *(text) }
  .data : { *(.data) }
  .data : { font0.o }
  .bss  : { *(.bss) }
  
  . = ALIGN(8);
  . = . + 0x1000; /* 4kB of svc stack space */
  svc_stack_top = .;
  . = . + 0x1000; /* 4kB of irq stack memory */
  irq_stack_top = .;
  . = . + 0x1000; /* 4kB of abt stack memory */
  abt_stack_top = .;
  . = . + 0x1000; /* 4kB of und stack memory */
  und_stack_top = .;
  
}


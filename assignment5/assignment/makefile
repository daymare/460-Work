
ARM = arm-none-eabi
CFLAGS = -nostartfiles -std=c99 -Wfatal-errors -w -c -mcpu=arm926ej-s -nostdlib -fno-builtin -nostdinc

# ensure the assignment is compiled then run it with qemu
run: t.bin
	qemu-system-arm -M versatilepb -m 512M -kernel t.bin  


# compile and link assignment into binary executable
t.bin: ts.s t.ld t.c *.c *.h u1.o
	$(ARM)-objcopy -I binary -O elf32-littlearm -B arm font0 font0.o
	nm -S -t d font0.o
	$(ARM)-as -mcpu=arm926ej-s -g ts.s -o ts.o
	$(ARM)-gcc $(CFLAGS) -g t.c -o t.o
	$(ARM)-ld -T t.ld ts.o t.o -Ttext 0x10000 -o t.elf
	$(ARM)-objcopy -O binary t.elf t.bin
	rm *.o *.elf 

# compile and link user mode program into binary executable
u1.o: us.s u1.c u.ld
	$(ARM)-objcopy -I binary -O elf32-littlearm -B arm font0 font0.o
	nm -S -t d font0.o
	$(ARM)-as -mcpu=arm926ej-s -g us.s -o us.o
	$(ARM)-gcc $(CFLAGS) -g u1.c -o u1.o
	$(ARM)-ld -T u.ld us.o u1.o -Ttext 0x80000000 -o u.elf
	$(ARM)-objcopy -O binary u.elf u.bin
	$(ARM)-objcopy -O elf32-littlearm -B arm u.bin u.o
	rm *.o *.elf 


clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.bin


ARM = arm-none-eabi
CFLAGS = -nostartfiles -std=c99 -Wfatal-errors -w -c -mcpu=arm926ej-s -nostdlib -fno-builtin -nostdinc

# ensure the assignment is compiled then run it with qemu
run: t.bin
	qemu-system-arm -M versatilepb -m 512M -kernel t.bin  


# assemble file ts.s into a binary file to be run by quemu
t.bin: ts.s t.ld t.c *.c *.h
	$(ARM)-objcopy -I binary -O elf32-littlearm -B arm font0 font0.o
	nm -S -t d font0.o
	$(ARM)-as -mcpu=arm926ej-s -g ts.s -o ts.o
	$(ARM)-gcc $(CFLAGS) -g t.c -o t.o
	$(ARM)-ld -T t.ld ts.o t.o -Ttext 0x10000 -o t.elf
	$(ARM)-objcopy -O binary t.elf t.bin

	rm *.o *.elf 

clean:
	rm *.o
	rm *.elf
	rm *.bin
	cp font font.o

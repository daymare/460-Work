# ensure the assignment is compiled then run it with qemu
run: t.bin
	qemu-system-arm -M versatilepb -kernel t.bin -m 128M -serial mon:stdio

ARM = arm-none-eabi

# assemble file ts.s into a binary file to be run by quemu
t.bin: ts.s t.ld t.c
	$(ARM)-as -mcpu=arm926ej-s ts.s -o ts.o  # assemble t.s to t.o
	$(ARM)-gcc -mcpu=arm926ej-s -fno-builtin -c t.c -o t.s
	$(ARM)-ld -T t.ld ts.o t.s -o t.elf # link t.o to t.elf
	#$(ARM)-nm t.elf # show symbols in t.elf
	$(ARM)-objcopy -O binary t.elf t.bin # objcopy t.elf to t.bin

clean:
	rm *.o
	rm *.elf
	rm *.bin
	cp font font.o

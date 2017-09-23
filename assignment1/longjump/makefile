# ensure the assignment is compiled then run it with qemu
run: assignment.bin
	qemu-system-arm -M versatilepb -kernel assignment.bin -m 128M -serial mon:stdio

ARM = arm-none-eabi

# assemble file ts.s into a binary file to be run by quemu
assignment.bin: assignment.s assignment.ld assignment.c
	$(ARM)-objcopy -I binary -O elf32-littlearm -B arm image1 image.o

	$(ARM)-as -o assignment.o assignment.s # assemble assignment.s to assignment.o
	$(ARM)-gcc -c assignment.c -o t.s
	$(ARM)-ld -T assignment.ld -o assignment.elf assignment.o t.s # link assignment.o to assignment.elf
	#$(ARM)-nm assignment.elf # show symbols in assignment.elf
	$(ARM)-objcopy -O binary assignment.elf assignment.bin # objcopy assignment.elf to t.bin

clean:
	rm *.o
	rm *.elf
	rm *.bin
	cp font.apple font.o

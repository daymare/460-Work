# ensure the assignment is compiled then run it with qemu
run: assignment.bin
	qemu-system-arm -M versatilepb -kernel assignment.bin -nographic -serial /dev/null

# assemble file ts.s into a binary file to be run by quemu
assignment.bin: assignment.s assignment.ld
	arm-none-eabi-as -o assignment.o assignment.s # assemble assignment.s to ts.o
	arm-none-eabi-ld -T assignment.ld -o assignment.elf assignment.o # link ts.o to assignment.elf
	arm-none-eabi-nm assignment.elf # show symbols in assignment.elf
	arm-none-eabi-objcopy -O binary assignment.elf assignment.bin # objcopy assignment.elf to t.bin

clean:
	rm *.o
	rm *.elf
	rm *.bin


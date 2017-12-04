#!/bin/bash



if [ $# \< 1 ]; then
   echo "make_and_copy.bash FILENAME (without .c)"
   exit
fi

arm-none-eabi-as -mcpu=arm926ej-s us.s -o us.o
arm-none-eabi-gcc -Wfatal-errors -w -c -nostdlib -fno-builtin -nostdinc -nostartfiles -std=c99 -c -mcpu=arm926ej-s -o $1.o $1.c #2> /dev/null
arm-none-eabi-ld -T u.ld us.o $1.o eoslib -Ttext=0x80000000 -o bin/$1

mount -o loop sdimage /mnt
cp -av bin/$1 /mnt/bin
#ls -l /mnt/bin
umount /mnt

rm *.o


#!/bin/bash

i=$1

if [ "$i" = "clean" ]
then
	echo "Deleting all object files and binaries..."
	rm objs/*.o
	rm objs/*.elf
	rm objs/*.bin
	exit
fi

if [ "$i" = "" ]
then 
	echo "The source files have been built..."
	echo -e "To copy files to sdcard \n Use: ./build.sh flash"
else
	echo "Action requested: $i"
	echo "Building all source files... and copying image into sdcard"
fi

 arm-none-eabi-as sources/boot.s -o objs/boot.o
 arm-none-eabi-gcc -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -std=c99 -Iincludes/ -c sources/gpio.c -o objs/gpio.o
 arm-none-eabi-gcc -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -std=c99 -Iincludes/ -c sources/uart.c -o objs/uart.o
 arm-none-eabi-gcc -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -std=c99 -Iincludes/ -c sources/main_lcd.c -o objs/main_lcd.o
 arm-none-eabi-ld -T linker.ld -o objs/myos.elf objs/boot.o objs/gpio.o objs/uart.o objs/main_lcd.o
 arm-none-eabi-objcopy objs/myos.elf -O binary objs/myos.bin
 arm-none-eabi-objdump -D objs/myos.elf > objs/myos.list

if [ "$i" = "flash" ]
then
	if [ -e /media/abhimanyu999p/boot/ ]
	then
		echo "sdcard present" 
	else
		echo -e "FIXME:\nsdcard not present or\npath of the sdcard/boot/ partition not correct"
		exit
	fi
	cp objs/myos.bin /media/abhimanyu999p/boot/kernel7.img
	ls -l  /media/abhimanyu999p/boot/kernel7.img
	umount /media/abhimanyu999p/*
	sync
fi



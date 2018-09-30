ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 


.PHONY: all
all : kernel7.img

boot.o : boot.s
	$(ARMGNU)-as boot.s -o boot.o

uart-bootloader.o : uart-bootloader.c defines.h
	$(ARMGNU)-gcc $(COPS) -c uart-bootloader.c -o uart-bootloader.o

kernel7.img : loader boot.o uart-bootloader.o
	$(ARMGNU)-ld boot.o uart-bootloader.o -T loader -o uart-bootloader.elf
	$(ARMGNU)-objdump -D uart-bootloader.elf > uart-bootloader.list
	$(ARMGNU)-objcopy uart-bootloader.elf -O binary kernel7.img

.PHONY: clean
clean: 
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.img


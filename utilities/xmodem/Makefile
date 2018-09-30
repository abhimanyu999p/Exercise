
CFLAGS = -Werror -O2 -Wall -c

.PHONY : all
all : gen/xmodem
 
gen/xmodem.o : src/xmodem.c
	gcc $(CFLAGS) -Iinc/ src/xmodem.c -o gen/xmodem.o

gen/serial.o : src/serial.c
	gcc $(CFLAGS) -Iinc/ src/serial.c -o gen/serial.o


gen/xmodem : gen/xmodem.o gen/serial.o
	gcc gen/xmodem.o gen/serial.o -o gen/xmodem

.PHONY : clean
clean :
	rm -f gen/*

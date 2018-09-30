obj-m += myGpio.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) \
	KBUILD_EXTMOD=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

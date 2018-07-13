obj-m = hello.o
KDIR := /home/joe/bootlin/linux_kernel/linux

all:
	make -C $(KDIR) M=$(shell pwd) modules
	
clean:
	make -C $(KDIR) M=$(shell pwd) clean

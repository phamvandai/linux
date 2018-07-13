# Build module
Set KDIR in Makefile to path of BBB kernel module 

	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-

# Copy module from PC to BBB
	
	tftp
	connect 192.168.1.5 69
	get hello.ko
	quit


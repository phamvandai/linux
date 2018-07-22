# Play with Beaglebone black

Very good source to start 

https://bootlin.com/docs/

# KERNEL BUILDING

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- omap2plus_defconfig 
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- uImage dtbs LOADADDR=0x80008000 -j4

# Tutorials

http://www.armhf.com/boards/beaglebone-black/bbb-sd-install/

http://www.bootembedded.com/beagle-bone-black/building-embedded-linux-system-using-mainline-kernel-for-beaglebone-black/

https://elinux.org/Building_for_BeagleBone

# BOOTARGS
uEnv.txt 

	bootpart=0:1
	devtype=mmc
	bootdir=
	bootfile=zImage
	bootpartition=mmcblk0p2
	set_mmc1=if test $board_name = A33515BB; then setenv bootpartition mmcblk1p2; fi
	set_bootargs=setenv bootargs console=ttyO0,115200n8 root=/dev/${bootpartition} rw 		rootfstype=ext4 rootwait
	uenvcmd=run set_mmc1; run set_bootargs;run loadimage;run loadfdt;printenv bootargs;bootz ${loadaddr} - ${fdtaddr}

# Test branch added


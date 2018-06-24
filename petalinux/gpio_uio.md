# Use GPIO UIO (Zedboard, petalinux 2015.2)

- Enable uio in kernel: petalinux-config -c kernel 
Userspace I/O drivers --> User I/O platform driver with generic IRQ handling --> Select built-in

- Modify device tree to use uio driver instead of xps-gio-driver
Open subsystems/linux/configs/device-tree/system-tops.dts and add following lines:

&btns_5bits {
	compatible = "generic-uio";
} ;

&leds_8bits {
	compatible = "generic-uio";
} ;

&sws_8bits {
	compatible = "generic-uio";
} ;

Those line will replace default attributes to use generic-uio

- Properly configure uio with kernel on startup:
petalinux-config --> Kernel bootargs --> Untick generate boot args automatically and type new args:

console=ttyPS0,115200 earlyprintk uio_pdrv_genirq.of_id=generic-uio

- Add new application & replace source by gpio-uio-test.c

- Build system 

- UIO devices will be list in /sys/class/uio 

- Demo: gpio-uio-test -d /dev/uio1 -o 255 


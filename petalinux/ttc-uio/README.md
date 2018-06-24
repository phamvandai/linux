# Example of using TTC0 with UIO

Need to add to system-top.dts 

	&ttc0 {
	
		compatible = "generic-uio";

	};

to enable uio for TTC.

We can verify information for uio devices in petalinux

cat /sys/class/uio/uio0/maps/map0/addr  - this must be physical address of TTC0 base register


To use UIO interrupt, the interrupt line must be connected in Vivado (for AXI devices).



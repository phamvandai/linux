# Source petalinux settings: 
source /<path_to_petalinux>/settings.sh

# Create project with existing petalinux BSP:
petalinux-create --type project --name zedboard --source Avnet-Digilent-Zedboard-v2015.2.1-final.bsp

# Configuration project
petalinux-config

# Configure kernel
petalinux-config -c kernel

# Configure rootfs
petalinux-config -c rootfs

# Build system
petalinux-build

# Create bootable images
Enable Vivado bootgen by source /path/to/vivado/settings.sh
petalinux-package --boot --format BIN --fsbl images/linux/zynq_fsbl.elf --fpga images/linux/download.bit --u-boot
Copy BOOT.BIN and image.ub

# Create new user app 
petalinux-create -t apps --name myapp --enable
Source will be generate in components/apps/

# Create prebuilt user app 
petalinux-create -t apps --template install --name myhello --enable
(Replace default app by user app (.elf) built by SDK. Then edit comment 
$(TARGETINST) -d data/myhello /bin/myhello

# Build and integrate app into system
petalinux-config -c rootfs  // Enable myapp

# Build application 
petalinux-build -c rootfs/hello

# Package packet 
petalinux-build -x package 

petalinux-build again
create bootable image again

# Modules
Build module separately petalinux-build -c my_module, find .ko file in 
build/linux/rootfs/targetroot/lib/modules/3.19.0-xilinx/extra
Send to target using ftp and install 

# Startup script 
https://www.xilinx.com/support/answers/55998.html
Note: No space in Makefile (only TAB)

# Some resources:
https://www.beyond-circuits.com/wordpress/tutorial/tutorial22/
embedded-centric.com


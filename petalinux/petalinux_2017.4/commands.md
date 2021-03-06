# Install Vivado/SDK/Petalinux 2017.4 tools in Ubuntu 16.04

## 1. Install Vivado/SDK
## 2. Install dependencies
	sudo dkpg --add-architecture i386
	sudo apt-get install zlib1g:i386

	sudo apt-get install python3 dos2unix iproute2 gawk xvfb git make net-tools libncurses5-dev 	tftpd lib32z1 libssl-dev flex bison libselinux1 gnupg wget diffstat chrpath socat xterm 	autoconf libtool tar unzip texinfo zlib1g-dev gcc-multilib build-essential libsdl1.2-dev 	libglib2.0-dev libsdl-dev build-essential gcc-multilib glib2.0 automake screen pax gzip 

## 3. Setup environment
change dash to bash
	sudo rm /bin/sh
	sudo ln -s /bin/bash /bin/sh

## 4. Run
need to source petalinux settings
	source <path_to_petalinux>/settings.sh
	echo $PETALINUX

Turn off webtalk statistics
	petalinux-util --webtalk off

## 5. Start
- Get hdf from Vivado
- Run petalinux
- Create petalinux project: 
	petalinux-create --type project --template zynqMP --name <my_linux_prj>
- Goto hdf folder & import hdf: 
	petalinux-config --get-hw-description -p <path_to_my_linux_prj>

## 6. Commands for ZynqMP

- build: 
	petalinux-build
- Config top level system: 
	petalinux-config
- Config kernel: 
	petalinux-config -c kernel
- Config rootfs:
	petalinux-config -c rootfs
- Create bootimage: 
	petalinux-package --boot --fsbl <path_to_fsbl_elf> --pmufw <path_to_pmu_fw> --fpga <path_to_bitstream>  --u-boot
- rebuild 
	petalinux-build -x mrproper

## 7. tftpboot
- install tftp on host
- uboot command:
	setenv ipaddr 192.168.1.100
	setenv serverip 192.168.1.5
	tftpboot image.ub
	bootm

## 8. Enable hidden packages
Enable in <project_dir>/project-spec/meta-user/recipes-core/images/petalinux-image.bbappend
add 
	IMAGE_INSTALL_append = " openssl"
	IMAGE_INSTALL_append = " openssl-misc"
	IMAGE_INSTALL_append = " openssl-conf"
	IMAGE_INSTALL_append = " openssl-dbg"
	IMAGE_INSTALL_append = " libcrypto"
	IMAGE_INSTALL_append = " openssl-dev"
	IMAGE_INSTALL_append = " libssl"
	IMAGE_INSTALL_append = " openssl-engines"

for openssl <project_dir>/project-spec/configs/rootfs_config

List of packages in <project_dir>/project-spec/configs/rootfs_config

## 9. Build app against built-in lib in XSDK

CONFIG LINKER 
	set linker flags --sysroot=<plnx_project_dir>/build/tmp/sysroots/plnx_aarch64
	set library search path
	set library include

CONFIG COMPILER
	set include paths


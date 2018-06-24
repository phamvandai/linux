## uboot commands
#  tftpboot (BOOT.bin & image.ub in /tftpboot on host)
setenv ipaddr 192.168.1.100
setenv serverip 192.168.1.5
saveenv
tftpboot image.ub
bootm

When linux image is too large, boot may fail. Need to set loadaddr bigger
setenv loadaddr 0x4000000
saveenv

# display saved environment variables
printenv



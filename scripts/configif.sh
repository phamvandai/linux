#!/bin/sh

# Configure network interface ip & gateway

ifconfig eth0 192.168.1.100 netmask 255.255.255.0 up
route add default gw 192.168.1.5

# Set nameserver to access internet
echo "nameserver 1.1.1.1" > /etc/resolv.conf


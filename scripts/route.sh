#!/bin/sh

# Enable ip forward on Linux via wlo1

echo 1 > /proc/sys/net/ipv4/ip_forward
INET="wlo1"
INETIP="$(ifconfig $INET | sed -n '/inet addr/{s/ .*addr://;s/ .*//;p}')"
iptables -t nat -A POSTROUTING -o $INET  -j SNAT --to-source $INETIP

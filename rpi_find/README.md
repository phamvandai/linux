
# How to find a raspberry pi in LAN?
A program will run on RPI in background at startup to listen at UDP Port 8668;
PC program will broadcast request string, RPI will reply with IP information.

1. Program on PC (pc_find.c)

pc_find pi_addr ACK 10 

	pi_addr : request string

	ACK	: ACK string

	10	: retry time

2. Program on RPI (pi_server.c)

Buid this program for RPI target

Add to file /etc/rc.local as follow

#Print the IP address

_IP=$(hostname -I) || true

if [ "$_IP" ]; then

  printf "My IP address is %s\n" "$_IP"

fi

sudo /home/pi/pi_server &

exit 0

#where /home/pi/pi_server is path to listen program 



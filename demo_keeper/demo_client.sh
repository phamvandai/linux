#!/bin/bash

if [ "$#" -eq 1 ]; then
	if [ "$1" = "run" ]; then
		./client 127.0.0.1 5555
	else if [ "$1" = "clean" ]; then
			rm client
		fi
	fi
else 
gcc -o client client.c -lssl -lcrypto
fi

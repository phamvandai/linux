#!/bin/bash

if [ "$#" -eq 1 ]; then
    if [ "$1" = "clean" ]; then
        rm initapp
        rm keeper
        rm mainapp
    else if [ "$1" = "run" ]; then
            ./initapp
        fi
    fi
else
    echo "Build initapp"
    g++ -o initapp initapp.c -Wall

    echo "Build keeper"
    g++ -o keeper keeper.c linuxutils.cpp -Wall

    echo "Build mainapp"
    g++ -o mainapp mainapp.c linuxutils.cpp -lssl -lcrypto -Wall
fi

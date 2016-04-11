#!/bin/bash

#script to have some easy work
make
sudo insmod char.ko
sudo dmesg -c
sudo chmod a+wr /dev/ALU
echo "Alok" > /dev/ALU 
dmesg
cat /dev/ALU
sudo rmmod char

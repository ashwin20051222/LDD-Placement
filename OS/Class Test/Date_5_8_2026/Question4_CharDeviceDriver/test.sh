#!/bin/sh
# Test script (assumes mychardev.ko is built separately)
# Insert module (if mychardev.ko present):
# sudo insmod mychardev.ko
# Create device node if not created by udev (example minor may vary):
# sudo mknod /dev/mychardev c 10 200

# Write and read examples (device created by misc or mknod):
# echo "Hello Driver" > /dev/mychardev
# cat /dev/mychardev

# Remove module:
# sudo rmmod mychardev

echo "Test script: see commented commands; build the module externally then run the commands to test." 

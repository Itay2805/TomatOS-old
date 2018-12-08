#!/bin/sh

export DISPLAY=0:0
qemu-system-i386 -s -S -kernel kernel.elf -m 4096
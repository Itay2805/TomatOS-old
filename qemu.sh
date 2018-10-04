export DISPLAY=0:0
qemu-system-i386 -s -S -kernel kernel.debug.elf -hda drive.qcow -m 4096
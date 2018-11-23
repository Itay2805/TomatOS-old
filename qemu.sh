export DISPLAY=0:0
qemu-system-i386 -boot d -cdrom kernel.iso -hda drive.qcow -m 4096 -vga std
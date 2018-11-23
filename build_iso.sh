echo "Building ISO (using kernel.debug.elf)"
mkdir iso
mkdir iso/boot
mkdir iso/boot/grub
cp kernel.debug.elf iso/boot/kernel.elf
echo 'set timeout=0'						>> iso/boot/grub/grub.cfg
echo 'set default=0'						>> iso/boot/grub/grub.cfg
echo ''									 	>> iso/boot/grub/grub.cfg
echo 'menuentry "TomatOS" {'				>> iso/boot/grub/grub.cfg
echo '  multiboot /boot/kernel.elf'		 	>> iso/boot/grub/grub.cfg
echo '  boot'							   	>> iso/boot/grub/grub.cfg
echo '}'									>> iso/boot/grub/grub.cfg
grub-mkrescue -p ./grub -v -o kernel.iso iso
rm -rf iso

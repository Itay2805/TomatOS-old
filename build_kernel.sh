#!/bin/bash
GCCPARAMS="-g -fdiagnostics-color=always -Wall -std=c++11 -fno-use-cxa-atexit -Ilibc -ITomatKernel -Wextra -m32 -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings"
CCFLAGS="-g -fdiagnostics-color=always -Wall -Wextra -Ilibc -ITomatKernel -std=gnu99 -nostdinc -fno-builtin -fno-stack-protector -march=i386 -m32"
NASMPARAMS="-g -f elf"
LDPARAMS="-melf_i386"

echo "Preparing kernel compilation"

echo "  Removing old binaries"
rm kernel.elf 2> /dev/null
rm kernel.debug.elf 2> /dev/null

echo "  Removing old build folder"
rm -rf build
mkdir build

echo "Compiling TomatKernel"
for d in $(find TomatKernel -type d 2> /dev/null)
do
	mkdir "build/$d"
	for f in $(find $d/*.c 2> /dev/null)
	do
		of="$f.o"
		of="build/$of"
		objects="$objects $of"
		echo "  $f > $of"
		gcc $CCFLAGS -c "./$f" -o "./$of" 
	done
	for f in $(find $d/*.cpp 2> /dev/null)
	do
		of="$f.o"
		of="build/$of"
		objects="$objects $of"
		echo "  $f > $of"
		g++ $GCCPARAMS -c "./$f" -o "./$of" 
	done
	for f in $(find $d/*.asm 2> /dev/null)
	do
		of="$f.o"
		of="build/$of"
		objects="$objects $of"
		echo "  $f > $of"
		nasm $NASMPARAMS "./$f" -o "./$of"
	done
done

if [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
	echo "Linking Kernel"
	ld $LDPARAMS -T TomatKernel/linker.ld -o kernel.debug.elf $objects
	
	echo "Removing debug symbols (for non debug version)"
	objcopy --strip-debug kernel.debug.elf kernel.elf
	
	# for now we don't care about iso
	#echo "Building ISO"
	#mkdir iso
	#mkdir iso/boot
	#mkdir iso/boot/grub
	#cp kernel.elf iso/boot/kernel.elf
	##echo 'set timeout=0'						>> iso/boot/grub/grub.cfg
	#echo 'set default=0'						>> iso/boot/grub/grub.cfg
	#echo ''									 >> iso/boot/grub/grub.cfg
	##echo 'menuentry "TomatOS" {'				>> iso/boot/grub/grub.cfg
	#echo '  multiboot /boot/kernel.elf'		 >> iso/boot/grub/grub.cfg
	#echo '  boot'							   >> iso/boot/grub/grub.cfg
	#echo '}'									>> iso/boot/grub/grub.cfg
	#grub-mkrescue --output=kernel.iso iso
	#rm -rf iso

elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
	echo "Skipping link (can't link on windows)"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
	echo "Skipping link (can't link on windows)"
fi

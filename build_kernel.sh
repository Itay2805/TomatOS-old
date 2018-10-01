#!/bin/bash
GCCPARAMS="-g -fdiagnostics-color=always -Wall -std=c++11 -fno-use-cxa-atexit -ITomatLib -Ilibc -ITomatKernel -Wextra -m32 -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings"
CCFLAGS="-g -fdiagnostics-color=always -Wall -Wextra -ITomatLib -Ilibc -ITomatKernel -std=gnu99 -nostdinc -fno-builtin -fno-stack-protector -march=i386 -m32"
NASMPARAMS="-g -f elf"
LDPARAMS="-melf_i386"

echo "Preparing kernel compilation"
should_link_kernel="false"
mkdir "build" 2> /dev/null

# Compiling Kernel
echo "Compiling TomatKernel"
for d in $(find TomatKernel -type d 2> /dev/null)
do
	mkdir "build/$d" 2> /dev/null

	# Build C files
	for f in $(find $d/*.c 2> /dev/null)
	do
		of="$f.o"
		of="build/$of"
		objects="$objects $of"
		
		if [ -e "build/$f.modtime" ]; then
			if [ "$(stat -c %y $f)" == "$(cat build/$f.modtime)" ]; then
				echo "	$f > not changed, skipping"
			else
				echo "  $f > $of"
				gcc $CCFLAGS -c "./$f" -o "./$of" 
				echo "$(stat -c %y $f)" >> "build/$f.modtime"
				should_link_kernel="true"
			fi
		else
			echo "  $f > $of"
			gcc $CCFLAGS -c "./$f" -o "./$of"
			echo "$(stat -c %y $f)" >> "build/$f.modtime"
			should_link_kernel="true"
		fi
	done

	# Build cpp files
	for f in $(find $d/*.cpp 2> /dev/null)
	do
		of="$f.o"
		of="build/$of"
		objects="$objects $of"

		if [ -e "build/$f.modtime" ]; then
			if [ "$(stat -c %y $f)" == "$(cat build/$f.modtime)" ]; then
				echo "	$f > not changed, skipping"
			else
				echo "  $f > $of"
				g++ $GCCPARAMS -c "./$f" -o "./$of" 
				echo "$(stat -c %y $f)" >> "build/$f.modtime"
				should_link_kernel="true"
			fi
		else
			echo "  $f > $of"
			g++ $GCCPARAMS -c "./$f" -o "./$of" 
			echo "$(stat -c %y $f)" >> "build/$f.modtime"
			should_link_kernel="true"
		fi
	done

	# Build asm files
	for f in $(find $d/*.asm 2> /dev/null)
	do
		of="$f.o"
		of="build/$of"
		objects="$objects $of"

		if [ -e "build/$f.modtime" ]; then
			if [ "$(stat -c %y $f)" == "$(cat build/$f.modtime)" ]; then
				echo "	$f > not changed, skipping"
			else
				echo "  $f > $of"
				nasm $NASMPARAMS "./$f" -o "./$of"
				echo "$(stat -c %y $f)" >> "build/$f.modtime"
				should_link_kernel="true"
			fi
		else
			echo "  $f > $of"
			nasm $NASMPARAMS "./$f" -o "./$of"
			echo "$(stat -c %y $f)" >> "build/$f.modtime"
			should_link_kernel="true"
		fi
	done
done

# Compile 

# Linking
if [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then

	echo $should_link_kernel

	if [ "$should_link_kernel" == "true" ]; then
		echo "Linking Kernel"
		ld $LDPARAMS -T TomatKernel/linker.ld -o kernel.debug.elf $objects
		echo "Removing debug symbols (for non debug version)"
		objcopy --strip-debug kernel.debug.elf kernel.elf
	else
		if [ -e "kernel.debug.elf" ]; then
			echo "Nothing changed, skipping kernel-link"
		else
			echo "No kernel.debug.elf file found, linking kernel"
			ld $LDPARAMS -T TomatKernel/linker.ld -o kernel.debug.elf $objects
		fi

		if [ -e "kernel.elf" ]; then
			echo "Non-Debug version already exists"
		else
			echo "Removing debug symbols (for non debug version)"
			objcopy --strip-debug kernel.debug.elf kernel.elf
		fi
	fi

elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
	echo "Skipping link (can't link on windows)"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
	echo "Skipping link (can't link on windows)"
fi

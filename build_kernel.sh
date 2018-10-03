#!/bin/bash
GCCPARAMS="-g -fdiagnostics-color=always -Wall -std=c++11 -fno-use-cxa-atexit -ITomatLib -Ilibc -ITomatKernel -Wextra -m32 -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings"
CCFLAGS="-g -fdiagnostics-color=always -Wall -Wextra -ITomatLib -Ilibc -ITomatKernel -std=gnu99 -nostdinc -fno-builtin -fno-stack-protector -march=i386 -m32"
NASMPARAMS="-g -f elf"
LDPARAMS="-melf_i386"

echo "Preparing kernel compilation"
rm kernel.debug.elf 2> /dev/null
rm kernel.elf 2> /dev/null

should_link_kernel="false"
mkdir "build" 2> /dev/null

# Currently there is a problem with the way this works... because if you edit a header file it will not affect the compilcation
# So anything which requires inlining will not re-compile...

process_file_c() {
	f=$1

	of="$f.o"
	of="build/$of"

	mkdir -p "$(dirname "$of")"
	
	if [ -e "build/$f.modtime" ]; then
		if [ "$(stat -c %y $f)" == "$(cat build/$f.modtime)" ]; then
			echo "	$f > not changed, skipping"
		else
			echo "  $f > $of"
			gcc $CCFLAGS -c "./$f" -o "./$of"

			rm "build/$f.modtime"
			echo "$(stat -c %y $f)" >> "build/$f.modtime"
		fi
	else
		echo "  $f > $of"
		gcc $CCFLAGS -c "./$f" -o "./$of"
		echo "$(stat -c %y $f)" >> "build/$f.modtime"
	fi
}

process_file_cpp() {
	f=$1

	of="$f.o"
	of="build/$of"

	mkdir -p "$(dirname "$of")"

	if [ -e "build/$f.modtime" ]; then
		if [ "$(stat -c %y $f)" == "$(cat build/$f.modtime)" ]; then
			echo "	$f > not changed, skipping"
		else
			echo "  $f > $of"
			g++ $GCCPARAMS -c "./$f" -o "./$of" 
			
			rm "build/$f.modtime"
			echo "$(stat -c %y $f)" >> "build/$f.modtime"
		fi
	else
		echo "  $f > $of"
		g++ $GCCPARAMS -c "./$f" -o "./$of" 
		echo "$(stat -c %y $f)" >> "build/$f.modtime"
	fi
}

process_file_asm() {
	f=$1

	of="$f.o"
	of="build/$of"

	mkdir -p "$(dirname "$of")"

	if [ -e "build/$f.modtime" ]; then
		if [ "$(stat -c %y $f)" == "$(cat build/$f.modtime)" ]; then
			echo "	$f > not changed, skipping"
		else
			echo "  $f > $of"
			nasm $NASMPARAMS "./$f" -o "./$of"

			rm "build/$f.modtime"
			echo "$(stat -c %y $f)" >> "build/$f.modtime"
		fi
	else
		echo "  $f > $of"
		nasm $NASMPARAMS "./$f" -o "./$of"
		echo "$(stat -c %y $f)" >> "build/$f.modtime"
	fi
}

process_folder() {
	d=$1

	mkdir "build/$d" 2> /dev/null

	# Build C files
	for f in $(find $d/*.c 2> /dev/null)
	do
		process_file_c $f &
	done

	# Build cpp files
	for f in $(find $d/*.cpp 2> /dev/null)
	do
		process_file_cpp $f &
	done

	# Build asm files
	for f in $(find $d/*.asm 2> /dev/null)
	do
		process_file_asm $f &		
	done

	# Wait for file processing
	for job in `jobs -p`
	do
		wait $job
	done
}

# Compiling Kernel
echo "Compiling TomatKernel"
for d in $(find TomatKernel -type d 2> /dev/null)
do
	process_folder $d & 
done

# Wait for folder processing
for job in `jobs -p`
do
	wait $job
done

objects="$(find ./build -type f -name \*.o)"

# Linking
if [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
	echo "Linking Kernel"
	ld $LDPARAMS -T TomatKernel/linker.ld -o kernel.debug.elf $objects
	echo "Removing debug symbols (for non debug version)"
	objcopy --strip-debug kernel.debug.elf kernel.elf
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
	echo "Skipping link (can't link on windows)"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
	echo "Skipping link (can't link on windows)"
fi

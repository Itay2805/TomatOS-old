# some values
PREFIX="/usr/local/i386elfgcc"
export PATH="$PREFIX/bin:$PATH"
export C_INCLUDE_PATH=./kernel/api/include/:./kernel/libc/
#export CFLAGS="-g -ffreestanding -Wall -Wextra -fno-exceptions -m32"
#export C_SOURCES=

TARGET=i386

CCFLAGS="-Wall -Wextra -std=gnu99 -nostdinc -fno-builtin -fno-stack-protector -march=$TARGET -m32"

# this is kept for debugging, so it will give me all the warnings possible
#CCFLAGS="-O3 -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Wuninitialized -Wconversion -Wstrict-prototypes -Wunsafe-loop-optimizations -std=gnu99 -nostdinc -fno-builtin -fno-stack-protector -Wmissing-prototypes -Wmissing-declarations -march=$TARGET -m32"

# cleanup
echo "Removing old build folder"
rm os-image.bin
rm -r build
mkdir build

objectFiles=""

# get all the c files in the kernel
echo "Compiling kernel: "
for d in $(find kernel/ -type d)
do
    mkdir "build/$d"
    for f in $(find $d/*.c)
    do
        of=`echo $f | sed 's/\(.*\)c/\1o/'`
        of="build/$of"
        objectFiles="$objectFiles $of"
        echo "  $f > $of"
        i386-elf-gcc $CCFLAGS -c "./$f" -o "./$of" 
    done
done
echo "  boot/kernel_entry.asm > build/kernel_entry.o"
nasm boot/kernel_entry.asm -f elf -o build/kernel_entry.o

# link kernel
echo "Linking kernel"
i386-elf-ld -o build/kernel.bin -Ttext 0x1000 build/kernel_entry.o $objectFiles --oformat binary

# compile the bootloader
echo "Compiling bootloader"
nasm ./boot/boot.asm -f bin -o ./build/boot.bin

# create a boot image
echo "Creating boot image - os-image.bin"
cat ./build/boot.bin ./build/kernel.bin > os-image.bin
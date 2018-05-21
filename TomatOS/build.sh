export C_INCLUDE_PATH=include:libc
GCCPARAMS="-Wall -std=c++11 -fno-use-cxa-atexit -Iinclude -Ilibc -Wextra -m32 -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings"
CCFLAGS="-Wall -Wextra -std=gnu99 -nostdinc -fno-builtin -fno-stack-protector -march=i386 -m32"
NASMPARAMS="-f elf"
LDPARAMS="-melf_i386"

# cleanup
clear 
clear

echo "Removing old build folder"
rm kernel.bin
rm kernel.iso
rm -rf build
mkdir build

echo "Compiling kernel"
objects=""

for d in $(find src/ -type d)
do
    mkdir "build/$d"
    for f in $(find $d/*.c)
    do
        of=`echo $f | sed 's/\(.*\)c/\1o/'`
        of="build/$of"
        objects="$objects $of"
        echo "  $f > $of"
        gcc $CCFLAGS -c "./$f" -o "./$of" 
    done
    for f in $(find $d/*.cpp)
    do
        of=`echo $f | sed 's/\(.*\)cpp/\1o/'`
        of="build/$of"
        objects="$objects $of"
        echo "  $f > $of"
        g++ $GCCPARAMS -c "./$f" -o "./$of" 
    done
	for f in $(find $d/*.asm)
    do
        of=`echo $f | sed 's/\(.*\)asm/\1o/'`
        of="build/$of"
        objects="$objects $of"
        echo "  $f > $of"
        nasm $NASMPARAMS "./$f" -o "./$of"
    done
done

echo "Compiling APIs"
mkdir build/include/
for d in $(find include/apis/ -type d)
do
    mkdir "build/$d"
    for f in $(find $d/*.c)
    do
        of=`echo $f | sed 's/\(.*\)c/\1o/'`
        of="build/$of"
        objects="$objects $of"
        echo "  $f > $of"
        gcc $CCFLAGS -c "./$f" -o "./$of" 
    done
    for f in $(find $d/*.cpp)
    do
        of=`echo $f | sed 's/\(.*\)cpp/\1o/'`
        of="build/$of"
        objects="$objects $of"
        echo "  $f > $of"
        g++ $GCCPARAMS -c "./$f" -o "./$of" 
    done
	for f in $(find $d/*.asm)
    do
        of=`echo $f | sed 's/\(.*\)asm/\1o/'`
        of="build/$of"
        objects="$objects $of"
        echo "  $f > $of"
        nasm $NASMPARAMS "./$f" -o "./$of"
    done
done

echo "Compiling LIBC"
for d in $(find libc/ -type d)
do
    mkdir "build/$d"
    for f in $(find $d/*.c)
    do
        of=`echo $f | sed 's/\(.*\)c/\1o/'`
        of="build/$of"
        objects="$objects $of"
        echo "  $f > $of"
        gcc $CCFLAGS -c "./$f" -o "./$of" 
    done
	for f in $(find $d/*.asm)
    do
        of=`echo $f | sed 's/\(.*\)asm/\1o/'`
        of="build/$of"
        objects="$objects $of"
        echo "  $f > $of"
        nasm $NASMPARAMS "./$f" -o "./$of"
    done
done


echo "Compiling CPPLIB"
for d in $(find cpplib/ -type d)
do
    mkdir "build/$d"
    for f in $(find $d/*.c)
    do
        of=`echo $f | sed 's/\(.*\)c/\1o/'`
        of="build/$of"
        objects="$objects $of"
        echo "  $f > $of"
        gcc $CCFLAGS -c "./$f" -o "./$of" 
    done
    for f in $(find $d/*.cpp)
    do
        of=`echo $f | sed 's/\(.*\)cpp/\1o/'`
        of="build/$of"
        objects="$objects $of"
        echo "  $f > $of"
        g++ $GCCPARAMS -c "./$f" -o "./$of" 
    done
	for f in $(find $d/*.asm)
    do
        of=`echo $f | sed 's/\(.*\)asm/\1o/'`
        of="build/$of"
        objects="$objects $of"
        echo "  $f > $of"
        nasm $NASMPARAMS "./$f" -o "./$of"
    done
done

echo "Linking"
ld $LDPARAMS -T linker.ld -o kernel.bin $objects

echo "Building ISO"
mkdir iso
mkdir iso/boot
mkdir iso/boot/grub
cp kernel.bin iso/boot/kernel.bin
echo 'set timeout=0'                        >> iso/boot/grub/grub.cfg
echo 'set default=0'                        >> iso/boot/grub/grub.cfg
echo ''                                     >> iso/boot/grub/grub.cfg
echo 'menuentry "TomatOS" {'                >> iso/boot/grub/grub.cfg
echo '  multiboot /boot/kernel.bin'         >> iso/boot/grub/grub.cfg
echo '  boot'                               >> iso/boot/grub/grub.cfg
echo '}'                                    >> iso/boot/grub/grub.cfg
grub-mkrescue --output=kernel.iso iso --xorriso="$HOME"/xorriso/xorriso-1.4.6/xorriso/xorriso
rm -rf iso

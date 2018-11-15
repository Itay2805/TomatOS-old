export CCFLAGS="-g -fdiagnostics-color=always -Wall -Wextra -I. -I../TomatLibc -std=gnu99 -nostdinc -fno-builtin -fno-stack-protector -march=i386 -m32"
export LDPARAMS="-melf_i386"

gcc $CCFLAGS -c gdt.c -o gdt.o 
gcc $CCFLAGS -c kernel.c -o kernel.o
ld $LDPARAMS -T link.ld -o kernel.debug.elf gdt.o kernel.o 

rm gdt.o
rm kernel.o

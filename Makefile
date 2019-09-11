all:
	nasm -f elf32 start.asm -o start.o
	nasm -f elf32 kernel/mtables.asm -o mtables.o
	gcc -Wall -g -Wno-unused-function -Wnonnull-compare -fno-stack-protector -mgeneral-regs-only -m32 -c kernel/kernel.c -o k.o
	ld -m elf_i386 -T link.ld -o boot/kernel.bin start.o k.o mtables.o
	grub-mkrescue -o Zildj1anOS.iso boot 2> /dev/null
	rm -rf isodir
	rm *.o
#	qemu-system-x86_64 -cdrom Zildj1anOS.iso
	qemu-system-x86_64 -kernel boot/kernel.bin -net none

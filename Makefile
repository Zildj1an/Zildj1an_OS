all:
	You need to install nasm,gcc,grub,xorriso &> /dev/null
	nasm -f elf32 start.asm -o start.o
	gcc -Wall -g -m32 -c kernel/kernel.c -o k.o
	ld -m elf_i386 -T link.ld -o boot/kernel.bin start.o k.o
	grub-mkrescue -o Zildj1anOS.iso boot 2> /dev/null
	rm -rf isodir
	rm start.o
	rm k.o
#	qemu-system-x86_64 -cdrom Zildj1anOS.iso
	qemu-system-x86_64 -kernel boot/kernel.bin -net none

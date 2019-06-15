CC=gcc

ifndef CFLAGS
CFLAGS=-g -ggdb -Wall -ansi -pedantic -ffreestanding
endif

all: kernel.elf

kernel.elf: start.o kernel.o
	ld -T link.ld -o $@ $^

start.o: start.asm
	nasm -f elf start.asm -o start.o

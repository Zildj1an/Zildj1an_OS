SRC_DIR = kernel/
ARCH_DIR = arch/

EXECUTABLE = kernel.bin
OBJ = k.o

CC = gcc
CFLAGS = -Wall -g -Wno-unused-function -Wnonnull-compare -fno-stack-protector -mgeneral-regs-only -m32 -c

LD = ld
LDFLAGS = -m elf_i386
LDFILE = link.ld

TFTP_PATH = /srv/tftp/

SRC_C = kernel.c


qemu: $(EXECUTABLE)
	qemu-system-x86_64 -kernel boot/kernel.bin -net none

net: $(EXECUTABLE)
	sudo cp boot/kernel.bin $(TFTP_PATH)
	sudo cp boot/grub.cfg $(TFTP_PATH)boot/grub/
	
$(EXECUTABLE): $(OBJ)
	$(LD) $(LDFLAGS) -T $(LDFILE) -o $@ *.o
	@if [ $(du -k boot/kernel.bin | grep -o '[0-9]*') -ge 800 ]; then\
		echo "Caution, kernel size approaching 1MB. Update MEM_SAFE_DISTANCE!";\
	fi

$(OBJ): $(SRC_DIR) arch
	$(CC) $(CFLAGS) $(SRC_DIR)$(SRC_C) -o $@

arch: $(SRC_DIR)$(ARCH_DIR)
	make -C $(SRC_DIR)$(ARCH_DIR)

clean:
	rm -rf isodir
	rm *.o

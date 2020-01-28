ROOT = /home/pablo/Documentos/Programación/Zildj1an_OS/


CC = nasm
FLAGS = -f elf32

SRCS = start.asm mtables.asm time.asm
OBJS = $(SRCS:.asm=.o)

all: $(SRCS) $(OBJS)

%.o: %.asm
	$(CC) $(FLAGS) $< -o $(ROOT)$@

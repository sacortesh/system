
# stub PXE : necessaire pour le demarrage via QEmu
PXE = boot.pxe

# noyau
KERNEL = kernel.bin

# on compile tous les fichiers assembleur et c du repertoire
SRCS = $(wildcard *.s *.S *.c)

# crt0.o doit etre linke en premier
OBJS = $(strip crt0.o $(filter-out crt0.o,$(notdir $(patsubst %.s,%.o,$(patsubst %.S,%.o,$(patsubst %.c,%.o,$(SRCS)))))))

CC = gcc
AS = gcc
LD = ld
OBJCOPY = objcopy
QEMU = /usr/libexec/qemu-kvm

CFLAGS = -m32 -Wall -Werror -g -gstabs -std=c99 -nostdinc -fno-stack-protector -I.
ASFLAGS = -m32 -DASSEMBLER -g -gstabs -I.
LDFLAGS = -melf_i386
QEMUOPTS = -no-kvm -net nic -net user,tftp=`pwd`,bootfile=$(PXE) -boot n -cpu pentium -rtc base=localtime -m 64M -S -s

# cible principale, on nettoie systematiquement le repertoire avant
.PHONY: all
all: clean kernel.bin

# ecran affichant les exceptions
task_dump_screen.o: empty.o task_dump_screen.txt
	$(OBJCOPY) empty.o --add-section=.task_dump_screen=task_dump_screen.txt --set-section-flags=.task_dump_screen=contents,alloc,load,data task_dump_screen.o

# generation du noyau
kernel.bin: kernel.lds $(OBJS) task_dump_screen.o
	$(LD) $(LDFLAGS) -e entry -Tkernel.lds $(OBJS) task_dump_screen.o -o $@

clean:
	$(RM) $(OBJS) $(KERNEL) task_dump_screen.o

run:
	$(QEMU) $(QEMUOPTS)


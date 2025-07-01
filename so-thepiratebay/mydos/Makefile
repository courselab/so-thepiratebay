# SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
# SPDX-FileCopyrightText: 2025 keny-azvd <kenner-azevedo@hotmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

CC=gcc
MAKE=make

dos="thepiratedos"

# Build the OS and an example user program.

# Link all objects needed by the OS.
$(dos).bin : bootloader.o bios1.o kernel.o kaux.o bios2.o logo.o syscall.o read_sector.o
	ld -melf_i386 -T tydos.ld --orphan-handling=discard $^ -o $@

# Link user program statically into the kernel (optional)
$(dos).bin : prog.o libtydos.o

# Rules to build objects from C code
%.o : %.c
	$(CC) -m16 -O0 --freestanding -fno-pic $(NO_CF_PROTECT) -c  $< -o $@

# Rules to build objects from Assembly (.S files preprocessed by gcc)
%.o : %.S
	$(CC) -m16 -c $< -o $@

bootloader.o : bios1.h kernel.h
kernel.o : bios1.h bios2.h kernel.h kaux.h
kaux.o: bios2.h kaux.h

$(dos).bin : .EXTRA_PREREQS = rt0.o tydos.ld

# Build user program(s)
progs = prog.bin

$(progs)  : %.bin : %.o libtydos.a
	ld -melf_i386 -T prog.ld --orphan-handling=discard $< -o $@

$(progs:%.bin=%.o) : %.o : %.c tydos.h
	$(CC) -m16 -O0 --freestanding -fno-pic $(NO_CF_PROTECT) -c $< -o $@

$(progs:%.bin=%.o) : tydos.h
$(progs:%.bin=%.o) : .EXTRA_PREREQS = prog.ld

# User library
libtydos.o: libtydos.c tydos.h
	$(CC) -m16 -O0 --freestanding -fno-pic $(NO_CF_PROTECT) -c  $< -o $@

libtydos.a : libtydos.o
	ar rcs $@ $^

# Compilar o read_sector em Assembly
read_sector.o: read_sector.S
	$(CC) -m16 -c read_sector.S -o read_sector.o

# Disco
disk.img:
	test -f disk.img || dd if=/dev/zero of=disk.img count=2880

copy:
	dd if=$(dos).bin of=disk.img obs=16 seek=1 conv=notrunc

mydosrun:
	qemu-system-i386 -fda disk.img

.PHONY: clean
clean:
	rm -f *.bin *.o *~ *.s *.a

# SYSeg's convenience rules
include bintools.mk


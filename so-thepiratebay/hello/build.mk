
this: hello.bin

main.o : main.c
	gcc -m16 -O1 -Wall -fno-pic -fno-asynchronous-unwind-tables -c $< -fno-builtin -fno-stack-protector -nostdinc -I. -o $@ 

libc.o : libc.S
	gcc -m16 -O1 -Wall -fno-pic -fno-asynchronous-unwind-tables -c $< -fno-builtin -fno-stack-protector -o $@

crt0.o : crt0.S
	gcc -m16 -O1 -Wall -fno-pic -fno-asynchronous-unwind-tables -c $< -fno-builtin -fno-stack-protector -o $@

hello.bin : main.o libc.o | crt0.o hello.ld
	gcc -m16 $^ -nostartfiles -nostdlib -T hello.ld -orphan-handling=discard -o $@




#
# Housekeeping
#

clean:
	rm -f  *.bin *.elf *.o *.s *.iso *.img *.i kh
	make clean-extra



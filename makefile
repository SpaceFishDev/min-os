KERN_OBJS := $(patsubst %.c,%.o,$(wildcard src/kernel/*.c src/kernel/*/*.c src/kernel/*/*/*.c))
CFLAGS =  -std=gnu99 -ffreestanding -O2 -Wall -Wextra
KERN_ASM_OBJS := $(patsubst %.asm,%.o,$(wildcard src/kernel/*.asm src/kernel/*/*.asm src/kernel/*/*/*.asm))

all: boot.o minos.bin 

boot.o:
	i686-elf-as src/bootloader/boot.s -o boot.o


$(KERN_OBJS): %.o: %.c
	i686-elf-gcc -c  $< -o $@ $(CFLAGS)
$(KERN_ASM_OBJS): %.o: %.asm
	nasm $< -o  $@ -felf 

minos.bin: $(KERN_OBJS)  $(KERN_ASM_OBJS)
	i686-elf-gcc -T linker.ld  -o $@ -ffreestanding -O2 -nostdlib $^ boot.o  

grub:
	grub-mkrescue -o minos.iso iso

run:
	qemu-system-i386 -kernel minos.bin

clean:
	rm $(KERN_OBJS)
	rm $(KERN_ASM_OBJS)
	rm *.o
	rm minos.bin
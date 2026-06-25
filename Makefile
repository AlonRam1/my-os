CC = gcc
LD = ld

CFLAGS = -m32 -ffreestanding -O0 -Wall -Wextra -fno-pic -fno-pie -fno-stack-protector

BUILD = build

all: iso

$(BUILD)/boot.o: src/boot.S
	mkdir -p $(BUILD)
	$(CC) -m32 -c src/boot.S -o $(BUILD)/boot.o

$(BUILD)/kernel.o: src/kernel.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c src/kernel.c -o $(BUILD)/kernel.o

$(BUILD)/kernel.elf: $(BUILD)/boot.o $(BUILD)/kernel.o linker.ld
	$(LD) -m elf_i386 -T linker.ld -nostdlib -o $(BUILD)/kernel.elf $(BUILD)/boot.o $(BUILD)/kernel.o

iso: $(BUILD)/kernel.elf grub.cfg
	mkdir -p iso/boot/grub
	cp $(BUILD)/kernel.elf iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o myos.iso iso

run: iso
	qemu-system-x86_64 -cdrom myos.iso

clean:
	rm -rf build iso myos.iso

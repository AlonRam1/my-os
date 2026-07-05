CC = gcc
LD = ld

CFLAGS = -m32 -ffreestanding -O0 -Wall -Wextra -fno-pic -fno-pie -fno-stack-protector -g

BUILD = build

all: iso

$(BUILD)/boot.o: src/boot.S
	mkdir -p $(BUILD)
	$(CC) -m32 -g -c src/boot.S -o $(BUILD)/boot.o

$(BUILD)/kernel.o: src/kernel.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c src/kernel.c -o $(BUILD)/kernel.o

$(BUILD)/vga.o: src/vga.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c src/vga.c -o $(BUILD)/vga.o

$(BUILD)/idt.o: src/idt.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c src/idt.c -o $(BUILD)/idt.o

$(BUILD)/isr.o: src/isr.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c src/isr.c -o $(BUILD)/isr.o

$(BUILD)/isr-asm.o: src/isr.S
	mkdir -p $(BUILD)
	$(CC) -m32 -g -c src/isr.S -o $(BUILD)/isr-asm.o

$(BUILD)/pic.o: src/pic.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c src/pic.c -o $(BUILD)/pic.o

$(BUILD)/pmm.o: src/pmm.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c src/pmm.c -o $(BUILD)/pmm.o

$(BUILD)/kernel.elf: $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/vga.o $(BUILD)/idt.o $(BUILD)/isr.o $(BUILD)/isr-asm.o $(BUILD)/pic.o $(BUILD)/pmm.o linker.ld
	$(LD) -m elf_i386 -T linker.ld -nostdlib -o $(BUILD)/kernel.elf $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/vga.o $(BUILD)/idt.o $(BUILD)/isr.o $(BUILD)/isr-asm.o $(BUILD)/pic.o $(BUILD)/pmm.o

iso: $(BUILD)/kernel.elf grub.cfg
	mkdir -p iso/boot/grub
	cp $(BUILD)/kernel.elf iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o myos.iso iso

run: iso
	qemu-system-i386 -cdrom myos.iso

debug: iso
	qemu-system-i386 \
		-cdrom myos.iso \
		-s \
		-S

clean:
	rm -rf build iso myos.iso

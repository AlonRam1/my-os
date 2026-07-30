CC = gcc
LD = ld

SRC = src
BUILD = build
INCLUDE = include

CFLAGS = -m32 \
	-ffreestanding \
	-O0 \
	-Wall \
	-Wextra \
	-fno-pic \
	-fno-pie \
	-fno-stack-protector \
	-g \
	-I$(INCLUDE) \
	-I$(SRC)

ASFLAGS = -m32 -g -I$(INCLUDE) -I$(SRC)

C_SOURCES = $(shell find $(SRC) -name "*.c")
ASM_SOURCES = $(shell find $(SRC) -name "*.S")

C_OBJECTS = $(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(C_SOURCES))
ASM_OBJECTS = $(patsubst $(SRC)/%.S,$(BUILD)/%.asm.o,$(ASM_SOURCES))

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)


all: iso


$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILD)/%.asm.o: $(SRC)/%.S
	mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@


$(BUILD)/kernel.elf: $(OBJECTS) linker.ld
	$(LD) -m elf_i386 -T linker.ld -nostdlib -o $@ $(OBJECTS)


iso: $(BUILD)/kernel.elf grub.cfg
	mkdir -p iso/boot/grub
	cp $(BUILD)/kernel.elf iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o myos.iso iso


run: iso
	qemu-system-i386 -cdrom myos.iso -drive file=disk.img,format=raw,if=ide

debug: iso
	qemu-system-i386 \
		-cdrom myos.iso \
		-s \
		-S


clean:
	rm -rf build iso myos.iso

.set MB2_MAGIC, 0xE85250D6
.set ARCH_I386, 0
.set HEADER_LEN, multiboot2_end - multiboot2_start

.section .multiboot
.align 8
multiboot2_start:
    .long MB2_MAGIC
    .long ARCH_I386
    .long HEADER_LEN
    .long -(MB2_MAGIC + ARCH_I386 + HEADER_LEN)

    .short 0
    .short 0
    .long 8
multiboot2_end:

.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:

.section .text
.global _start
.extern kmain

_start:
    cli
    mov $stack_top, %esp

    and $0xFFFFFFF0, %esp   # force 16-byte alignment (important)

    call kmain

.hang:
    hlt
    jmp .hang

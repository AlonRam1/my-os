#THIS CODE SECTION IS JUST FOR GRUB TO IDENTIFY THIS AS A VALID KERNEL
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

#--------------------END OF GRUB IDENTIFY SECTION---------------------------------#

.section .bss

.align 16 #aligns the next label (stack_bottom) to align with a 16 byte boundry (aka round to make label_address(mod 16) = 0) 

stack_bottom:
    .skip 16384 #reserve 16KB of memory for stack
stack_top: #stack grows downward -> "stack_top" is really the bottom of the stack (where we start pushing)

.section .text
.global _start
.extern kmain

_start:
    cli

    mov $stack_top, %esp

    mov $0xCAFEBABE, %eax
    push %eax
    pop %eax

    call kmain
.hang:
    hlt
    jmp .hang

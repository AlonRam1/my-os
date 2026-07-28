.global gdt_load

gdt_load:
    mov 4(%esp), %eax #put gdt pointer (passed as argument from C code) in eax

    lgdt (%eax) #store gdt in cpu

    #initialize segment registers to 0x10 (kernel data segment)
    #NOTE: the CPU reads from the segment registers to get GDT data (limit, granularity, etc.) in different contexts (code, data, stack, etc.)
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    ljmp $0x08, $flush #set cs segment register by jumping

flush:
    ret

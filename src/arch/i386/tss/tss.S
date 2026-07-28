.global tss_load

tss_load:
    mov $0x28, %ax
    ltr %ax
    ret

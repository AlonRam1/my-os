.global enter_user_mode

.extern user_test

enter_user_mode:

    cli

    mov $0x23, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # user SS
    push $0x23

    # user ESP
    push $0x90000

    # EFLAGS
    pushf
    pop %eax
    or $0x200, %eax
    push %eax

    # user CS
    push $0x1b

    # user EIP
    push $user_test

    iret

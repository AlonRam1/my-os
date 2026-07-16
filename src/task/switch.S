.global switch_interrupt_context

switch_interrupt_context:
    mov 4(%esp), %eax
    mov %eax, %esp
    popa
    iret

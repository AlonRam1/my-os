.global switch_task
.global switch_interrupt_context

switch_task:

    pusha

    mov 36(%esp), %eax
    mov %esp, (%eax)

    mov 40(%esp), %esp

    popa

    ret

switch_interrupt_context:
    mov 4(%esp), %eax
    mov %eax, %esp
    popa
    iret

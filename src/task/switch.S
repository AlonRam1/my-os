.global switch_task

switch_task:

    pusha

    mov 36(%esp), %eax
    mov %esp, (%eax)

    mov 40(%esp), %esp

    popa

    ret

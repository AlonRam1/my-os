.global isr0
.extern isr0_handler

isr0:
    pusha //push general purpose registers into stack
    call isr0_handler
    popa //pop general purpose registers from stack
    iret //interrupt return

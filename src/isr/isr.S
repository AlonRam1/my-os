.global isr0
.global isr1
.global isr3
.global isr4
.global isr5
.global isr6
.global isr8
.global isr13
.global isr14
.global isr128
.global irq0
.global irq1

.extern isr0_handler
.extern isr1_handler
.extern isr3_handler
.extern isr4_handler
.extern isr5_handler
.extern isr6_handler
.extern isr8_handler
.extern isr13_handler
.extern isr14_handler
.extern irq0_handler
.extern irq1_handler

isr0:
    pusha //push general purpose registers into stack
    call isr0_handler 
    popa //pop general purpose registers from stack
    iret //interrupt return

    
isr1:
    pusha //push general purpose registers into stack
    call isr1_handler 
    popa //pop general purpose registers from stack
    iret //interrupt return

isr3:
    pusha //push general purpose registers into stack
    call isr3_handler 
    popa //pop general purpose registers from stack
    iret //interrupt return

isr4:
    pusha //push general purpose registers into stack
    call isr4_handler 
    popa //pop general purpose registers from stack
    iret //interrupt return

isr5:
    pusha //push general purpose registers into stack
    call isr5_handler 
    popa //pop general purpose registers from stack
    iret //interrupt return


isr6:
    pusha //push general purpose registers into stack
    call isr6_handler 
    popa //pop general purpose registers from stack
    iret //interrupt return


isr8:
    pusha
    call isr8_handler
    popa
    iret

//INTERRUPTS WITH RETURN VALUES    

isr13:
    cli
    pusha         

    call isr13_handler	
    addl $2, 36(%esp) //add 2 bytes to the return address (that is in esp) to skip the instruction that triggered the interrupt.   

    popa                
    add $4, %esp        
    iret                

isr14:
    cli
    pusha

    push %esp
    call isr14_handler
    add $4, %esp

    popa
    add $4, %esp
    iret

isr128:
    pusha

    push %esp
    call isr128_handler
    add $4, %esp

    popa
    iret   

irq0:
    cli

    pusha

    mov %esp, %eax #store current task stack in eax

    #call the C handler with eax as the parameter
    push %eax
    call irq0_handler

    add $4, %esp #restore stack

    mov %eax, %esp #move next task esp into esp

    popa

    iret

irq1:
    cli
    pusha
    call irq1_handler
    popa
    movb $0x20, %al
    outb %al, $0x20
    iret

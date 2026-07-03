.global isr0
.global isr1
.global isr3
.global isr4
.global isr5
.global isr6
.global isr8
.global isr13
.global isr14

.extern isr0_handler
.extern isr1_handler
.extern isr3_handler
.extern isr4_handler
.extern isr5_handler
.extern isr6_handler
.extern isr8_handler
.extern isr13_handler
.extern isr14_handler

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
    # 2. Advance the saved EIP past the 2-byte 'mov %eax, %ds' instruction
    # With pusha active, EIP is at [esp + 32 (pusha) + 4 (error code)] = [esp + 36]
    addl $2, 36(%esp)   

    popa                
    add $4, %esp        
    iret                

isr14:
    cli
    pusha               

    call isr14_handler
    # 2. Advance the saved EIP past the 2-byte 'mov %eax, %ds' instruction
    # With pusha active, EIP is at [esp + 32 (pusha) + 4 (error code)] = [esp + 36]
    addl $2, 36(%esp)   

    popa               
    add $4, %esp        
    iret                



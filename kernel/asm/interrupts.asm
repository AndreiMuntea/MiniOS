%include "definitions.inc"

GLOBAL IntLoadIdt
GLOBAL IntCommonISR

[Bits 64]

; void IntLoadIdt(void* IdtDescriptor)
IntLoadIdt:
    push rbp
    mov rbp, rsp

    call IntSetupPIC
    lidt [rcx]      ; ABI call so in rcx is first parameter

    leave
    ret


; void IntSetupPIC(void)
IntSetupPIC:
    push rbp 
    mov rbp, rsp 
    SAVE_REGS

    xchg bx, bx 
    ; SEND ICW1
    mov al, PIC_ICW1
    out PIC_MASTER_COMMAND, al 
    out PIC_SLAVE_COMMAND,  al

    ; send ICW2
    mov al, PIC_ICW2_MASTER
    out PIC_MASTER_DATA, al 

    mov al, PIC_ICW2_SLAVE
    out PIC_SLAVE_DATA, al 

    ; send ICW3
    mov al, PIC_ICW3_MASTER
    out PIC_MASTER_DATA, al 

    mov al, PIC_ICW3_SLAVE
    out PIC_SLAVE_DATA, al 

    ; send ICW4
    mov al, PIC_ICW4
    out PIC_MASTER_DATA, al 
    out PIC_SLAVE_DATA,  al 

    RESTORE_REGS
    leave
    ret 

; void IntCommonISR(void)
IntCommonISR:
    cli
    
    push rbx 
    xchg bx, bx 
    pop rbx

    sti 
    iretq 
%include "definitions.inc"

GLOBAL IntLoadIdt

[Bits 64]

; void IntLoadIdt(void* IdtDescriptor)
IntLoadIdt:
    push rbp
    mov rbp, rsp
    SAVE_REGS 

    xchg bx, bx
    lidt [rcx]      ; abi call so in rcx is first parameter

    RESTORE_REGS
    leave
    ret
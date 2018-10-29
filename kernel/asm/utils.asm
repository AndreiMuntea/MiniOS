GLOBAL DebugBreak
GLOBAL Halt


; void DebugBreak(void)
DebugBreak:
    push rbp
    mov rbp, rsp

    xchg bx, bx

    leave
    ret

; void Halt(void)
Halt:
    push rbp 
    mov rbp, rsp 

    .halt:
    hlt
    jmp .halt

    leave 
    ret 
GLOBAL DebugBreak
DebugBreak:
    push rsp
    mov rbp, rsp 
    ;pusha 

    xchg bx, bx

    ;popa 
    pop rbp
    ret
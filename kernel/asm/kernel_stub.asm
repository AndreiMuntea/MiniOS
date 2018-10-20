extern main

[bits 32]
KernelStub:
    push ebp 
    mov ebp, esp
    
    call main

    pop ebp
    ret
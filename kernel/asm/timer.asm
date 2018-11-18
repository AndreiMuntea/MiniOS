%include "definitions.inc"

global TimerClockTick
global TimerGetClockTicks
global TimerSleep

[Bits 64]
gClockTicks dq 0

; void TimerClockTick(void)
TimerClockTick:
    push rbp 
    mov rbp, rsp 
    
    lock inc QWORD [gClockTicks]

    leave 
    ret 

; QWORD TimerGetClockTicks(void)
TimerGetClockTicks:
    push rbp 
    mov rbp, rsp 

    mov rax, [gClockTicks]

    leave 
    ret 

; void TimerSleep(QWORD Milliseconds)
TimerSleep:
    push rbp 
    mov rbp, rsp 
    SAVE_REGS

    call TimerGetClockTicks     ; Initial clock ticks
    mov rbx, rax 

    .Sleep:
        call TimerGetClockTicks ; Current clock ticks
        sub rax, rbx 
    cmp rax, rcx                ; Check if we slept enough ms
    jnae .Sleep

    RESTORE_REGS
    leave 
    ret 
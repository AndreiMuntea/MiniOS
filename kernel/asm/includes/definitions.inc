%macro SAVE_REGS 0
    push rax 
    push rbx 
    push rcx 
    push rdx 
    push rsi 
    push rdi 
    push rbp 
    push r8 
    push r9
    push r10 
    push r11 
    push r12 
    push r13 
    push r14
    push r15 
    pushf
%endmacro

%macro RESTORE_REGS 0
    popf
    pop r15
    pop r14 
    pop r13 
    pop r12 
    pop r11 
    pop r10 
    pop r9 
    pop r8 
    pop rbp 
    pop rdi 
    pop rsi 
    pop rdx 
    pop rcx 
    pop rbx 
    pop rax 
%endmacro


%define PIC_MASTER_COMMAND   0x20
%define PIC_MASTER_DATA      0x21

%define PIC_SLAVE_COMMAND    0xA0
%define PIC_SLAVE_DATA       0xA1


%define PIC_ICW1        00010001b
; WITH_ICW4 | CASCADE
%define PIC_ICW2_MASTER 00100000b
; MAPPED AT 32
%define PIC_ICW2_SLAVE  00101000b
; MAPPED AT 40
%define PIC_ICW3_MASTER 00000100b   
; IRQ2 WILL BE SLAVE
%define PIC_ICW3_SLAVE  00000010b
; NOTIFY THE SLAVE 
%define PIC_ICW4        00000001b
; 8086 MODE


; ICW - initialization command word

; ICW1
;|0|0|0|1|x|0|x|x|
;        |   | +--- with ICW4 (1) or without (0)
;        |   +----- one controller (1), or cascade (0)
;        +--------- triggering by level (level) (1) or by edge (edge) (0)

; ICW2
;|x|x|x|x|x|0|0|0|
; | | | | |
; +----------------- base address for interrupts vectors

; ICW3 - MASTER
;|x|x|x|x|x|x|x|x|
; | | | | | | | |
; +------------------ slave controller connected to the port yes (1), or no (0)

; ICW3 - SLAVE
;|0|0|0|0|0|x|x|x|  
;           | | |
;           +-------- Slave ID which is equal to the master port

; ICW4
;|0|0|0|x|x|x|x|x|
;       | | | | +---- mode 0=8085, 1=8086
;       | | | +------ mode "automatic end of interrupt" AEOI (1)
;       | | +-------- mode buffered slave (0) or master (1)
;       | +---------- mode buffered (1)
;       +------------ mode "fully nested" (1)

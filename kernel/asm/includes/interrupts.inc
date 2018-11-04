
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

%define PIC_EOI		    0x20	

%define KEYBOARD_PORT   0x60

%define TIMER_DESIRED_FREQUENCY 100

%define TIMER_BASE_FREQUENCY    1193180

%define TIMER_COMMAND_PORT      0x43

; CHANNEL 0 port is bounded to IRQ0
%define TIMER_CHANNEL0_PORT     0x40   


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


STRUC TRAP_FRAME
    .RAX:   resq 1
    .RBX:   resq 1
    .RCX:   resq 1
    .RDX:   resq 1 
    .RSI:   resq 1 
    .RDI:   resq 1 
    .RBP:   resq 1 
    .R8:    resq 1
    .R9:    resq 1
    .R10:   resq 1
    .R11:   resq 1
    .R12:   resq 1
    .R13:   resq 1
    .R14:   resq 1
    .R15:   resq 1
    .RIP:   resq 1
    .FLAGS: resq 1 
ENDSTRUC

%macro CREATE_TRAP_FRAME 1
    mov [%1 + TRAP_FRAME.RAX], rax
    mov [%1 + TRAP_FRAME.RBX], rbx
    mov [%1 + TRAP_FRAME.RCX], rcx
    mov [%1 + TRAP_FRAME.RCX], rcx
    mov [%1 + TRAP_FRAME.RCX], rcx
    mov [%1 + TRAP_FRAME.RCX], rcx
    mov [%1 + TRAP_FRAME.RCX], rcx
    mov [%1 + TRAP_FRAME.RDX], rdx
    mov [%1 + TRAP_FRAME.RSI], rsi
    mov [%1 + TRAP_FRAME.RDI], rdi
    mov [%1 + TRAP_FRAME.RBP], rbp
    mov [%1 + TRAP_FRAME.R8],  r8
    mov [%1 + TRAP_FRAME.R9],  r9
    mov [%1 + TRAP_FRAME.R10], r10
    mov [%1 + TRAP_FRAME.R11], r11
    mov [%1 + TRAP_FRAME.R12], r12
    mov [%1 + TRAP_FRAME.R13], r13
    mov [%1 + TRAP_FRAME.R14], r14
    mov [%1 + TRAP_FRAME.R15], r15
    
    push QWORD [rsp]
    pop  QWORD [%1 + TRAP_FRAME.RIP]

    pushf
    pop QWORD [%1 + TRAP_FRAME.FLAGS]
%endmacro
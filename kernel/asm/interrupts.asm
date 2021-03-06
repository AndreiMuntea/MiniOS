%include "definitions.inc"
%include "interrupts.inc"

GLOBAL IntLoadIdt
GLOBAL IntCommonISR
GLOBAL IntCriticalISR
GLOBAL IntKeyboardISR
GLOBAL IntTimerISR

extern KeyboardKeyPressed
extern TimerClockTick
extern IntDumpTrapFrame

[Bits 64]
gTrapFrame: ISTRUC TRAP_FRAME
	AT TRAP_FRAME.RAX,      dq 0
	AT TRAP_FRAME.RBX,      dq 0
	AT TRAP_FRAME.RCX,      dq 0
	AT TRAP_FRAME.RDX,      dq 0
	AT TRAP_FRAME.RSI,      dq 0
	AT TRAP_FRAME.RDI,      dq 0
	AT TRAP_FRAME.RBP,      dq 0
	AT TRAP_FRAME.R8,       dq 0
	AT TRAP_FRAME.R9,       dq 0
	AT TRAP_FRAME.R10,      dq 0
	AT TRAP_FRAME.R11,      dq 0
	AT TRAP_FRAME.R12,      dq 0
	AT TRAP_FRAME.R13,      dq 0
	AT TRAP_FRAME.R14,      dq 0
	AT TRAP_FRAME.R15,      dq 0
	AT TRAP_FRAME.FLAGS,    dq 0
IEND

; void IntLoadIdt(void* IdtDescriptor)
IntLoadIdt:
	push rbp
	mov rbp, rsp
	cli 

	call IntSetupPIC
	call IntSetupTimer
	lidt [rcx]      ; ABI call so in rcx is first parameter

	sti 
	leave
	ret


; void IntSetupPIC(void)
IntSetupPIC:
	push rbp 
	mov rbp, rsp 
	SAVE_REGS

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

	; Mask the interrupts
	mov al, 0xFC        ; Keyboard and Timer enabled
	out PIC_MASTER_DATA, al 

	mov al, 0xFF
	out PIC_SLAVE_DATA,  al 

	RESTORE_REGS
	leave
	ret 

; void IntSetupTimer(void)
IntSetupTimer:
	push rbp 
	mov rbp, rsp 
	SAVE_REGS

	mov al, 0x36    ; SQUARE-WAVE mode | 16 bit counter | lobyte and hibyte
	out TIMER_COMMAND_PORT, al 

	; calculate divisor
	xor rdx, rdx 
	mov rax, TIMER_BASE_FREQUENCY
	mov rbx, TIMER_DESIRED_FREQUENCY
	div rbx 

	; Send low byte
	out TIMER_CHANNEL0_PORT, al 
	
	; Send high byte
	rol ax, 8 
	out TIMER_CHANNEL0_PORT, al 

	RESTORE_REGS
	leave
	ret 

;void IntCriticalISR(void)
IntCriticalISR:
	cli

	CREATE_TRAP_FRAME gTrapFrame
	mov rcx, gTrapFrame
	call IntDumpTrapFrame

	DEBUG_BREAK
	hlt

	sti 
	iretq 

; void IntCommonISR(void)
IntCommonISR:
	cli
	SAVE_REGS

	mov al, PIC_EOI
	out PIC_MASTER_COMMAND, al

	RESTORE_REGS
	sti 
	iretq 

; void IntKeyboardISR(void)
IntKeyboardISR:
	cli
	SAVE_REGS

	in al, KEYBOARD_PORT
	
	movzx rcx, al
	call KeyboardKeyPressed

	mov al, PIC_EOI
	out PIC_MASTER_COMMAND, al

	RESTORE_REGS
	sti 
	iretq 

; void IntTimerISR(void)
IntTimerISR:
	cli
	SAVE_REGS

	call TimerClockTick

	mov al, PIC_EOI
	out PIC_MASTER_COMMAND, al

	RESTORE_REGS
	sti 
	iretq 
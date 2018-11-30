%include "definitions.inc"

GLOBAL DebugBreak
GLOBAL Halt
GLOBAL TestTrapFrame


; void DebugBreak(void)
DebugBreak:
	push rbp
	mov rbp, rsp

	DEBUG_BREAK

	leave
	ret

; void Halt(void)
Halt:
	push rbp 
	mov rbp, rsp 

	; Interrupts and exceptions can "wake" a processor from halt state
	.halt:
	hlt
	jmp .halt

	leave 
	ret 

; void TestTrapFrame(void)
TestTrapFrame:
	push rbp 
	mov rbp, rsp 
	SAVE_REGS

	mov r8,  0xDEADC0DEDEADC0DE
	mov r9,  0xDEADC0DEDEADC0DE
	mov r10, 0xDEADC0DEDEADC0DE
	mov r11, 0xDEADC0DEDEADC0DE
	mov r12, 0xDEADC0DEDEADC0DE
	mov r13, 0xDEADC0DEDEADC0DE
	mov r14, 0xDEADC0DEDEADC0DE
	mov r15, 0xDEADC0DEDEADC0DE

	mov rsi, 0x123456789
	mov rdi, 0xABCDEF

	xor rbx, rbx 
	xor rax, rax 
	xor rdx, rdx 
	xor rcx, rcx 

	div rcx

	RESTORE_REGS
	leave
	ret
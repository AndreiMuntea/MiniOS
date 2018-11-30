%include "definitions.inc"

%define PML4T           101000h
%define PDPT            102000h
%define PDT             103000h
; At 104000h there are page tables for identity map (first 2 mb)
%define PT              105000h 

; We allocate at 1000000h (16 mb)
; We will allocate a maximum of 64 pages of 4KB each
%define BASE_ADDRESS    1000000h
%define PAGE_SIZE       4096
%define MAX_PAGES       64

GLOBAL MemInit
GLOBAL MemUninit
GLOBAL MemAllocPage
GLOBAL MemFreePage

[Bits 64]
; If a bit is cleared it means that the page is free; otherwise is set
; For now we use only 64 pages 
gPagesBitmap dq 0

; void MemInit()
MemInit:
	push rbp 
	mov rbp, rsp 
	SAVE_REGS

	; Clear 4 kb of memory for page table
	mov edi, PT
	mov ecx, 1024
	xor eax, eax
	rep stosd  

	; PDT[8] = will be marked present => here will be our base address (16 mb)
	mov dword [PDT + 8 * 8],  PT | 011b  ; user mode / R-W / PDT present

	; Set up Page table
	mov eax, PT
	mov ebx, BASE_ADDRESS | 010b ; user mode / R-W / Page table not present 
	mov ecx, 512
	
	; Set up page table entries for allocation - 16 MB - 18 MB
	.SetEntry:
		mov [eax], ebx 
		add ebx, 4096   ; 4kb
		add eax, 8      ; move to the next page
	loop .SetEntry


	RESTORE_REGS
	leave 
	ret 

; BYTE MemAllocPage()
MemAllocPage:
	push rbp 
	mov rbp, rsp 

	mov rax, [gPagesBitmap] ; rax = gPagesBitmap
	not rax                 ; rax = !gPagesBitmap
	bsf rax, rax            ; rax = LSB(!gPagesBitmap)
	jnz .FoundPage          ; ZF set if no bit was set => all pages are allocated

	.NoPageFound:
		mov rax, -1
	jmp .Return

	.FoundPage:
		; Mark page present
		bts [gPagesBitmap], rax
		bts dword [PT + 8 * rax], 0    

		mov rcx, rax 
		call MemInvalidateTranslationBuffer        
	jmp .Return

	.Return:
	leave 
	ret 

; void MemFreePage(BYTE PageIndex)
MemFreePage:
	push rbp 
	mov rbp, rsp 
	SAVE_REGS

	; RCX - PageIndex
	cmp rcx, MAX_PAGES
	jae .Return

	; Mark page not present 
	btr [gPagesBitmap], rcx
	btr dword [PT + 8 * rcx], 0  
	
	; Invalidate translation buffer for given page
	call MemInvalidateTranslationBuffer

	.Return:
	RESTORE_REGS
	leave 
	ret 

; void MemUninit()
MemUninit:
	push rbp 
	mov rbp, rsp 
	SAVE_REGS

	xor rcx, rcx 
	.FreePage:
		call MemFreePage
		inc rcx 
	cmp rcx, MAX_PAGES 
	jne .FreePage
	
	; PDT[8] = will be marked not present
	btr dword [PDT + 8 * 8], 0 

	; Clear 4 kb of memory
	mov edi, PT
	mov ecx, 1024
	xor eax, eax
	rep stosd  

	RESTORE_REGS
	leave 
	ret 

; void MemFreePage(BYTE PageIndex)
MemInvalidateTranslationBuffer:
	push rbp 
	mov rbp, rsp 
	SAVE_REGS

	; RCX - PageIndex
	shl rcx, 12
	add rcx, BASE_ADDRESS
	invlpg [rcx]

	RESTORE_REGS
	leave 
	ret 
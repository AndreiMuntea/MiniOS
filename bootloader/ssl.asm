%include "bootloader.inc"

[bits 16]
[org ssl_origin]
SecondStageLoaderEntryPoint:
    ; Entering 32 bits Protected mode
    cli                                 ; Disable interrupts
    lgdt [GdtDescriptor32]              ; Load GDT register with start address of Global Descriptor Table
    mov eax, cr0            
    or al, 1                            ; Set PE (Protection Enable) bit in CR0 (Control Register 0)
    mov cr0, eax 

    ; Perform far jump to selector 08h (offset into GDT, pointing at code_descriptor) to load CS with proper PM32 descriptor
    jmp CS_SELECTOR:ProtectedModeEntryPoint     


[bits 32]
ProtectedModeEntryPoint:
    mov eax, DS_SELECTOR
    mov ds, ax
    mov ss, ax 
    mov es, ax 
    mov fs, ax 
    mov gs, ax 

    ; Move kernel from origin to base
    mov esi, kernel_origin
    mov edi, kernel_base
    mov ecx, 512 * 30
    cld 
    rep movsb

    ; Activating long mode
    mov eax, cr0
    and eax, 7FFFh          ; Disable paging (clear CR0.PG bit - bit 31)  
    mov cr0, eax 

    ; Clear 16 kb of memory for page tables
    mov edi, PML4T
    mov ecx, 4096
    xor eax, eax
    rep stosd       

    ; PML4T[0] = PDT
    mov dword [PML4T],  PDPT | 011b    ; user mode / R-W / PML4E present
    
    ; PDPT[0] = PDT 
    mov dword [PDPT],   PDT  | 011b    ; user mode / R-W / PDPT present

    ; PDT[0] = PT 
    mov dword [PDT],    PT   | 011b    ; user mode / R-W / PDT present

	; Set up PT
	mov eax, PT
	mov ebx, 011b ; user mode / R-W / Page table present 
    mov ecx, 512
    
    ; Set up indentity map - first 2 mb is identity map
    .SetEntry:
        mov [eax], ebx 
        add ebx, 4096   ; 4kb
        add eax, 8      ; move to the next page
    loop .SetEntry

    ; PDPT[1] = PDT  - so 400000h memory points here too (1gb)
	mov dword [PDPT + 8],   PDT | 011b  ; user mode / R-W / PDPT present

    ; PML4T[1] = PDPT - so 8000000000h memory points here too (512 gb)
    mov dword [PML4T + 8],  PDPT | 011b ; user mode / R-W / PDPT present

    mov eax, PML4T
	mov cr3, eax              ; Set CR3 to point to PML4T address

    mov eax, cr4
    or eax, 1 << 5            ; Enable physical-address extensions (set CR4.PAE bit - bit 5)
    mov cr4, eax 

    mov ecx, 0xC0000080       ; Set the C-register to 0xC0000080, which is the IA32_EFER MSR.
    rdmsr
    or eax, 1 << 8            ; Enable long mode (set IA32_EFER.LME bit - bit 8)
    wrmsr        

    lgdt [GdtDescriptor64]    ; Load GDT register with start address of Global Descriptor Table

    mov eax, cr0
    or eax, (1<<31) | (1<<0)  ; Enable paging (set CR0.PG bit - bit 31 and CR0.PM bit - bit 0)  
    mov cr0, eax 

    ; Perform far jump to selector 08h (offset into GDT, pointing at code_descriptor) to load CS with proper 64cs descriptor
    jmp CS_SELECTOR:Realm64 


[bits 64]
Realm64:
    mov rax, DS_SELECTOR
    mov ds, ax
    mov ss, ax 
    mov es, ax 
    mov fs, ax 
    mov gs, ax  

    mov rax, TestPagination
    call rax 

    mov rax, TestPagination + 40000000h
    call rax

    mov rax, TestPagination + 8000000000h
    call rax

    mov rax, kernel_base + 8000000000h
    call rax

TestPagination:
    ret


GdtDescriptor32:
    .size       dw  Gdt32.end - Gdt32 - 1   ; The size of the table subtracted by 1. This is because the maximum value of size is 65535, while the GDT can be up to 65536 bytes
    .base       dd  Gdt32                   ; The linear address of the table itself

Gdt32:
    .null: ISTRUC GlobalDescriptorTableEntry
        AT GlobalDescriptorTableEntry.segment_limit, dw 0
        AT GlobalDescriptorTableEntry.base_low,      dw 0
        AT GlobalDescriptorTableEntry.base_middle,   db 0
        AT GlobalDescriptorTableEntry.access,        db 0
        AT GlobalDescriptorTableEntry.granularity,   db 0
        AT GlobalDescriptorTableEntry.base_high,     db 0
    IEND
    .code_descriptor: ISTRUC GlobalDescriptorTableEntry
        AT GlobalDescriptorTableEntry.segment_limit, dw 0FFFFh
        AT GlobalDescriptorTableEntry.base_low,      dw 0
        AT GlobalDescriptorTableEntry.base_middle,   db 0
        AT GlobalDescriptorTableEntry.access,        db 10011010b       ; Execute and write + DPL 0
        AT GlobalDescriptorTableEntry.granularity,   db 11001111b
        AT GlobalDescriptorTableEntry.base_high,     db 0
    IEND
    .data_descriptor: ISTRUC GlobalDescriptorTableEntry
        AT GlobalDescriptorTableEntry.segment_limit, dw 0FFFFh
        AT GlobalDescriptorTableEntry.base_low,      dw 0
        AT GlobalDescriptorTableEntry.base_middle,   db 0
        AT GlobalDescriptorTableEntry.access,        db 10010010b       ; Read and write + DPL 0
        AT GlobalDescriptorTableEntry.granularity,   db 11001111b
        AT GlobalDescriptorTableEntry.base_high,     db 0
    IEND
.end: 


GdtDescriptor64:
    .size       dw  Gdt64.end - Gdt64 - 1   ; The size of the table subtracted by 1. This is because the maximum value of size is 65535, while the GDT can be up to 65536 bytes
    .base       dq  Gdt64                   ; The linear address of the table itself

Gdt64:
    .null: ISTRUC GlobalDescriptorTableEntry
        AT GlobalDescriptorTableEntry.segment_limit, dw 0                                    
        AT GlobalDescriptorTableEntry.base_low,      dw 0
        AT GlobalDescriptorTableEntry.base_middle,   db 0
        AT GlobalDescriptorTableEntry.access,        db 0
        AT GlobalDescriptorTableEntry.granularity,   db 0
        AT GlobalDescriptorTableEntry.base_high,     db 0
    IEND
    .code_descriptor: ISTRUC GlobalDescriptorTableEntry
        AT GlobalDescriptorTableEntry.segment_limit, dw 0               
        AT GlobalDescriptorTableEntry.base_low,      dw 0               
        AT GlobalDescriptorTableEntry.base_middle,   db 0                                                               
        AT GlobalDescriptorTableEntry.access,        db 10011000b                              
        AT GlobalDescriptorTableEntry.granularity,   db 00101111b                        
        AT GlobalDescriptorTableEntry.base_high,     db 0
    IEND
    .data_descriptor: ISTRUC GlobalDescriptorTableEntry
        AT GlobalDescriptorTableEntry.segment_limit, dw 0
        AT GlobalDescriptorTableEntry.base_low,      dw 0
        AT GlobalDescriptorTableEntry.base_middle,   db 0
        AT GlobalDescriptorTableEntry.access,        db 10010010b                                               
        AT GlobalDescriptorTableEntry.granularity,   db 0               
        AT GlobalDescriptorTableEntry.base_high,     db 0
    IEND
.end: 

; Fill the sector
times 512 - ($ - $$) db 0   ; $$ is the address of the beginning of the current section and $ is the current address   
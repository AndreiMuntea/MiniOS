%include "bootloader.inc"

[bits 16]
[org origin]
BootPoint:
	; Clear segment registers
	xor ax, ax
	mov ds, ax      
	mov es, ax      
	mov ss, ax      
	mov fs, ax  
	mov gs, ax

	; Initialize stack pointer
	mov sp, origin

	; Enable interrupts
	sti

	; Load into RAM second stage loader sector
	mov ah, 2           ; read sector interrupt
	mov al, 1           ; read 1 sector
	mov bx, ssl_origin  ; where to load sector; we load it right after the boot sector
	mov ch, 0           ; cylinder
	mov cl, 2           ; read the second sector; we skip sector 1 because it is the boot sector
	mov dh, 0           ; head number
	mov dl, 80h         ; 80h - disk drive
	int 13h
	jc Error

	; cylinders=128, heads=16, sectors=32
	; Load into RAM the kernel
	mov ah, 2               ; read sector interrupt
	mov al, 30              ; read 30 sectors
	mov bx, kernel_origin   ; where to load sector; we load it right after the boot sector
	mov ch, 0               ; cylinder
	mov cl, 3               ; read the second sector; we skip sector 1 because it is the boot sector
	mov dh, 0               ; head number
	mov dl, 80h             ; 80h - disk drive
	int 13h
	jnc ssl_origin

Error:
	; Fill the sector - must have 512 bytes                   
	times 510 - ($ - $$) db 0   ; $$ is the address of the beginning of the current section and $ is the current address   
	db 0x55, 0xAA               ; magic word for boot sector
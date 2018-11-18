%include "definitions.inc"

GLOBAL DiskReadSector

[Bits 64]
;=============================================================================
; Our disk configuration: cylinders=128, heads=16, sectors=32
; This routine will read 1 sector (512 bytes) from specified Head and Cylinder
; void DiskReadSector(WORD Cylinder, BYTE SectorIndex, BYTE Head, BYTE* OutputBuffer)
;=============================================================================
DiskReadSector
    push rbp 
    mov rbp, rsp 
    SAVE_REGS

    ; Cylinder     = RCX
    ; SectorIndex  = RDX
    ; Head         = R8
    ; OutputBuffer = R9

    ; Write any required parameters to the Features, Sector Count, Sector Number, Cylinder and Drive/Head registers.


    ; SECTOR COUNT register  port:  0x1F2
    ; This register specifies the number of sectors of data to be transferred during read/write sector commands
    ; We will read one sector
    mov al,   1
    out 1F2h, al
    
    ; SECTOR NUMBER register  port:  0x1F3
    ; This register contains the starting sector number for any disc access
    mov rax,  rdx 
    out 1F3h, al

    ; CYLINDER LOW register port:  0x1F4
    ; This register contains the eight least significant bits of the starting cylinder address for any disc access
    mov rax,  rcx
    out 1F4h, al

    ; CYLINDER HIGH register port:  0x1F5
    ; This register contains the most significant bits of the starting cylinder address for any disc access
    mov rax,  rcx
    xchg al,  ah
    out 1F4h, al

    ; DRIVE/HEAD register  port:  0x1F6
    ; |1|x|1|x|x|x|x|x|
    ;  | | | | +---------- HEAD      - is the 4-bit address used to select the head
    ;  | | | |------------ DRV       - the bit used to select the drive. Master is 0. Slave is 1
    ;  | | |-------------- RESERVED  - must be 1
    ;  | |---------------- LBA       - LBA (logical block address) for 1, CHS for 0
    ;  |------------------ RESERVED  - must be 1
    mov rax,  r8         
    or  al,   10100000b   ; Master DRV | CHS mode 
    out 1F6h, al          


    ; Write the command code to the Command register.

    ; COMMAND register  port:  0x1F7        !WRITE ONLY!
    ; This eight-bit register contains the host command. When this register is written, the drive immediately begins executing the command
    mov al, 20h ; Read Sectors (w/retry)
    out 1F7h, al

    ; Now The drive sets BSY and prepares for data transfer. When the drive is ready to accept a block of data, it sets DRQ and clears BSY. When the host detects DRQ is set to 1, the host writes one block of data from the Data register.

    ; STATUS register  port:  0x1F7         !READ ONLY! 
    ; |x|x|x|x|x|x|x|x|
    ;  | | | | | | | +---- ERR      - error bit. It is set to 1 when the previous command ended in some type of error
    ;  | | | | | | |------ IDX      - index bit. This bit usually contains a 0, except once per disc revolution when it is toggled from 0 to 1 and back to 0
    ;  | | | | | |-------- CORR     - corrected data bit. It is set to 1 when a correctable data error has been encountered and the data has been corrected
    ;  | | | | |---------- DRQ      - data request bit. It is set to 1 when the drive is ready to transfer a word or byte of data between the host and the data port
    ;  | | | |------------ DSC      - drive seek complete bit. It is set to 1 when the disc drive is not seeking
    ;  | | |-------------- DWF      - drive write fault bit. When there is a write fault error, this bit is set to 1 and is not changed until the Status register is read by the host, at which time the bit again indicates the current write fault status.
    ;  | |---------------- DRDY     - drive ready indicator bit. This bit is set to 0 at power up and remains set at 0 until the drive is ready to accept a command.
    ;  |------------------ BSY      - is the busy bit. It is set to 1 whenever the drive has access to the command block
    ; This register contains either the drive status or the controller status. It is updated at the completion of each command
    
    ; Wait for completion
    .checkBusy:
    in al, 1F7h
    test al, 8
    jz .checkBusy

    ; DATA register  port:  0x1F0 
    ; The host can only access this register when the DRQ bit in the status register is set to 1. All transfers use 16-bit words 
    mov rcx, 256    ; no words read
    mov rdx, 1F0h   
    mov rdi, r9     ; destination bufer
    rep insw        ; input from port to string 

    RESTORE_REGS
    leave
    ret 
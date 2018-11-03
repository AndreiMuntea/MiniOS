; LONG MODE 

;=======================================================================================
; 4-KB Page Translation
; 63                  48 47                   39 38                 30 29                 21 20               12 11              0
; |                     |                       |                     |                     |                   |                 |
; 
; Bits 63:48  - Sign extend
; Bits 47:39  - Page Map Level 4 offset (PML4)
; Bits 38:30  - Page Directory Pointer offset
; Bits 29:21  - Page Directory offset
; Bits 20:12  - Page Table offset
; Bits 11:0   - Physical page offset (4KB physical page)
;=======================================================================================    

;=======================================================================================
; COMMON BITS DEFINITIONS
; PCD   - Page level cache disable: PCD = 0 => the table is cacheable
;                                       = 1 => the table is not cacheable
;
; PWT   - Page level writethrough:  PWT = 0 => writeback caching policy
;                                       = 1 => writethrough caching policy
;
; AVL   - Available bits : Bits are not interpreted by the processor 
;                          and are available for use by the system software
; 
; A     - Accessed bit : Indicates whether the page translation table or physical page 
;                        to which entry points has been accessed. This bit is set to 1 
;                        by the processor the first time the table or physical page 
;                        is either read from or written to. It is never cleared by the 
;                        processor. Software can clear this bit when  it needs to track 
;                        the frequency of table or physical-page accesses.
;
; P     - Present bit : Indicates whether the page-translation table or physical page
;                       is loaded in physical memory. 
;                       P = 0 => the table or physical page is not loaded
;                         = 1 => the table or physical page is loaded
; 
; R/W   - Read/Write bit: Controls read/write access to all physical pages mapped
;                         by the table entry.
;                         R/W = 0 => access is restricted to read only
;                             = 1 => read and write access is allowed
;
; U/S   - User/Supervisor bit: Controls user (CPL 3) access to all physical pages mapped
;                              by the table entry. 
;                              U/S = 0 => access restricted to supervisor level (CPL 0, 1, 2)
;                                  = 1 => both user and supervisor access is allowed
;
; D     - Dirty bit : Indicates whether the physical page to which this entry points has
;                     been written. Set to 1 by the processor the first time there is a
;                     write to the physical page. It is never cleared by the processor.
;                     Software can clear this bit when  it needs to track the frequency 
;                     physical-page writes.
;
; PS    - Page size bit :    Present in page directoy entries and long-mode page directory
;                         pointer entries. When the PS bit is set in PDPE (page directory pointer entry)
;                         or page directory entry (PDE), that entry is the lowest level
;                         of page translation hierarchy. When the PS bit is cleared to 0
;                         in all levels above the page table entry (PTE), the lowest level
;                         of the page-translation hierarchy is the page-table entry, and the
;                         physical-page size is 4KB. 
;                            Physical-page size is determined as follows:
;                         -If EFER.LMA=1 and PDPE.PS=1, the physical-page size is 1 Gbyte.
;                         -If CR4.PAE=0  and PDE.PS=1,  the physical-page size is 4 Mbytes. 
;                         -If CR4.PAE=1  and PDE.PS=1,  the physical-page size is 2 Mbytes.
;
; G     - Global page bit: Present only in the lowest level of the page-translation hierarchy
;                          Use of the G bit requires the page-global enable bit
;                          in CR4 to be set to 1 (CR4.PGE=1). It indicates whether
;                          the physical page is a global page
;
; PAT   - Page-Attribute Table bit :     This bit is only present in the lowest level of 
;                                    the page-translation hierarchy, as follows:
;                                    -If the lowest level is a PTE (PDE.PS=0), PAT occupies bit 7
;                                    -If the lowest level is a PDE (PDE.PS=1) or PDPE (PDPE.PS=1), PAT occupies bit 12. 
;                                        It extends the page-table entry format and enhances the
;                                    capabilities provided by the PCD and PWT page-level cache controls
;                                    allowing memory-type characterization based on the virtual (linear) address
;=======================================================================================

;=======================================================================================
; CR0 register
;
; Bit 0 - PE (Protected mode enable) - PE = 1 => system is in protected mode
;                                         = 0 => system is in real mode
; Bit 1	 - MP (Monitor co-processor) - Controls interaction of WAIT/FWAIT instructions with TS flag in CR0
; Bit 2	 - EM (Emulation)            - If set, no x87 floating point unit present, if clear, x87 FPU present
; Bit 3	 - TS (Task switched)     	 - Allows saving x87 task context upon a task switch only after x87 instruction used
; Bit 4	 - ET (Extension type)   	 - On the 386, it allowed to specify whether the external math coprocessor was an 80287 or 80387
; Bit 5	 - NE (Numeric error) 	     - Enable internal x87 floating point error reporting when set, else enables PC style x87 error detection
; Bit 16 - WP (Write protect) 	     - When set, the CPU can't write to read-only pages when privilege level is 0
; Bit 18 - AM (Alignment mask)	     - Alignment check enabled if AM set, AC flag (in EFLAGS register) set, and privilege level is 3
; Bit 29 - NW (Not-write through)	 - Globally enables/disable write-through caching
; Bit 30 - CD (Cache disable) 	     - Globally enables/disable the memory cache
; Bit 31 - PG (Paging)    	         - If 1, enable paging and use the CR3 register, else disable paging
;
;=======================================================================================

;=======================================================================================
; CR3 register
; 63                  52 51                   12 11        5 4 3 2 1 0
; |                     |                       |                     |
; 
; CR3 is used to point to PML4 (Page map level 4) table
; Bits 63:52  - Reserved, Must be zero
; Bits 51:12  - Points to PML4 base address which is alligned to a 4-KB boundary (assumes bits 11:0 below to be 0)
; Bits 11:5   - Reserved, Must be zero
; Bit  4      - Page level cache disable (PCD) Bit; PCD = 0 => the table is cacheable 
;                                                         1 => the table is not cacheable
; Bit  3      - Page level writethrough (PWT) Bit; PWT = 0 => writeback caching policy
;                                                        1 => writethrough caching policy 
; Bits 2:0    - Reserved, Must be zero
;=======================================================================================

;=======================================================================================
; CR4 register
;
; Bit 0	 - VME	Virtual 8086 Mode Extensions	             - If set, enables support for the virtual interrupt flag (VIF) in virtual-8086 mode.
; Bit 1	 - PVI	Protected-mode Virtual Interrupts	         - If set, enables support for the virtual interrupt flag (VIF) in protected mode.
; Bit 4	 - PSE	Page Size Extension	                         - If unset, page size is 4 KiB, else page size is increased to 4 MiB 
;                                                              (if PAE is enabled or the processor is in Long Mode this bit is ignored).
; Bit 5	 - PAE	Physical Address Extension	                 - If set, changes page table layout to translate 32-bit virtual addresses into extended 36-bit physical addresses.
; Bit 7	 - PGE	Page Global Enabled	                         - If set, address translations (PDE or PTE records) may be shared between address spaces.
; Bit 20 - SMEP  Supervisor Mode Execution Protection Enable - If set, execution of code in a higher ring generates a fault
; Bit 21 - SMAP	Supervisor Mode Access Prevention Enable	 - If set, access of data in a higher ring generates a fault
;=======================================================================================

;=======================================================================================
; EFER (Extended Feature Enable Register)
; Bit  0       - SCE (System Call Extensions)
; Bits 1–7     - Reserved
; Bit  8	   - LME (Long Mode Enable)
; Bit  9	   - Reserved
; Bit  10	   - LMA (Long Mode Active)
; Bit  11	   - NXE (No-Execute Enable)
; Bit  12	   - SVME (Secure Virtual Machine Enable)
; Bit  13	   - LMSLE (Long Mode Segment Limit Enable)
; Bit  14	   - FFXSR (Fast FXSAVE/FXRSTOR)
; Bit  15	   - TCE (Translation Cache Extension)
;=======================================================================================

;=======================================================================================
; PML4 - 1 entry: 512 * 1GB = 512 GB
; 63 62                  52 51                   12 11  9 8 7 6 5 4 3 2 1 0
; |  |                     |                       |                       |
; 
; Bit 63 :    NX
; Bits 62:52  Available -> 512 entries (1 entry = 8 bytes)
; Bits 51:12  Page-Directory-Pointer Base address
; Bits 11:9   AVL 
; Bit  8      Must be 0
; Bit  7      Must be 0
; Bit  6      Ignored
; Bit  5      A
; Bit  4      PCD
; Bit  3      PWT
; Bit  2      U/S
; Bit  1      R/W
; Bit  0      P
;=======================================================================================

;=======================================================================================
; PPDE - 1 entry = 512 * 2MB = 1GB
; 63 62                  52 51                   12 11  9 8 7 6 5 4 3 2 1 0
; |  |                     |                       |                       |
; 
; Bit 63 :    NX
; Bits 62:52  Available -> 512 entries (1 entry = 8 bytes) 
; Bits 51:12  Page-Directory Base Address
; Bits 11:9   AVL 
; Bit  8      Ignored
; Bit  7      PS Must be 0 - 4KB address translation
; Bit  6      Ignored
; Bit  5      A
; Bit  4      PCD
; Bit  3      PWT
; Bit  2      U/S
; Bit  1      R/W
; Bit  0      P
;=======================================================================================

;=======================================================================================
; PDE - 1 entry = 512 B * 4kb = 2 MB
; 63 62                  52 51                   12 11  9 8 7 6 5 4 3 2 1 0
; |  |                     |                       |                       |
; 
; Bit 63 :    NX
; Bits 62:52  Available -> 512 entries (1 entry = 8 bytes)
; Bits 51:12  Page-Table Base Address
; Bits 11:9   AVL 
; Bit  8      Ignored
; Bit  7      PS Must be 0 - 4KB address translation
; Bit  6      Ignored
; Bit  5      A
; Bit  4      PCD
; Bit  3      PWT
; Bit  2      U/S
; Bit  1      R/W
; Bit  0      P
;=======================================================================================

;=======================================================================================
; PTE - 1 entry = 4KB
; 63 62                  52 51                   12 11  9 8 7 6 5 4 3 2 1 0
; |  |                     |                       |                       |
; 
; Bit 63 :    NX
; Bits 62:52  Available -> 512 entries (1 entry = 8 bytes)
; Bits 51:12  Physical-Page Base Address 
; Bits 11:9   AVL 
; Bit  8      G
; Bit  7      PAT
; Bit  6      D
; Bit  5      A
; Bit  4      PCD
; Bit  3      PWT
; Bit  2      U/S
; Bit  1      R/W
; Bit  0      P
;=======================================================================================


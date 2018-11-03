%define origin 7C00h
%define ssl_origin 7E00h
%define kernel_origin 8000h
%define kernel_base     110000h
%define PML4T           101000h
%define PDPT            102000h
%define PDT             103000h
%define PT              104000h
%define FIELD_OFFSET(Structure, Field) ((Field) - (Structure))


STRUC GlobalDescriptorTableEntry
    .segment_limit:  resw  1    
    .base_low:       resw  1   
    .base_middle:    resb  1    
    .access:         resb  1
	;=======================================================================================
	; ACCESS
	;  7 6 5 4 3 2 1 0 
	; |0|0|0|0|0|0|0|0|
	; 
	; Bit 0   : Access bit (Used with Virtual Memory)                                   
	; Bit 1   : 0 -> Read only (Data segments) / Execute only (Code segments)           
	;           1 -> Read and write (Data segments) / Execute and write (Code segments)
	; Bit 2   : Conforming (C) Bit Setting this bit to 1 identifies the code segment as conforming. 
    ;           When control is transferred to a higher-privilege conforming code-segment (C=1) from a lower-privilege code segment 
	; Bit 3   : Executable segment (0 for Data segments, 1 for Code segments)           
	; Bit 4   : Descriptor bit (0 for System descriptor, 1 for Code or Data descriptor) 
	; Bit 5-6 : Descriptor Privilege level (Ring 0 - 3)
	; Bit 7   : Segment is in memory (Used with Virtual Memory)
	;=======================================================================================	 
    .granularity:    resb  1
	;=======================================================================================
	; GRANULARITY
	;  7 6 5 4 3 2 1 0 
	; |0|0|0|0|0|0|0|0|
	; 
	; Bit 0-3 : Bits 16-19 of the segment limit                 
	; Bit 4   : Reserved for OS use                             
	; Bit 5   : Reserved should be 0
	; Bit 6   : Segment type: 0 - 16 bits, 1 - 32 bits                      
	; Bit 7   : Granularity : 0 - None, 1 - limit gets multiplied by 4k  
	;=======================================================================================
    .base_high:      resb  1    
ENDSTRUC
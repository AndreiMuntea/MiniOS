#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include "definitions.h"

#define IDT_MAX_ENTRIES                 256
#define CODE_SEGMENT_DESCRIPTOR         0x8
#define INTERRUPT_GATE_TYPE             0x8E        // P = 1b; DPL = 00b; Reserved = 0b; type = 1110b -> code | conforming | executable and readable | not accessed

#pragma pack(push, 1)
typedef struct _IDT_GATE_ENTRY
{
    WORD  OffsetLow;        // OFFSET      is a 64 bit value which represents the address of the entry point of the ISR
    WORD  Selector;         // SELECTOR    is a 16 bit value and must point to the valid descriptor in GDT
    BYTE  Ist;              // IST         Bits 0-2   : Offset into the Interrupt Stack Table. If all 3 bits are 0, IST not used
                            //             BITS 3-7   : Reserved - MBZ
    BYTE  Flags;            // FLAGS       Bits 0-3   : GateType
                            //             Bit  4     : Reserved - MBZ 
                            //             Bits 5-6   : DPL
                            //             Bit  7     : Presence bit
    WORD  OffsetMid;
    DWORD OffsetHigh;
    DWORD Reserved;         // Must be 0
}IDT_GATE_ENTRY, *PIDT_GATE_ENTRY;

typedef struct _IDT
{
    IDT_GATE_ENTRY Entries[IDT_MAX_ENTRIES];
}IDT, *PIDT;

typedef struct _IDT_DESCRIPTOR
{
    WORD    IdtSize;
    PIDT    IdtBase;
}IDT_DESCRIPTOR, *PIDT_DESCRIPTOR;
#pragma pack(pop)

void
InitializeIdtEntry(
    PIDT_GATE_ENTRY Entry,
    QWORD           Offset,
    WORD            Selector,
    BYTE            Flags
);

void 
IntInitializeIdt(
    PIDT    Idt
);

void
IntInitializeIdtDescriptor(
    PIDT_DESCRIPTOR IdtDescriptor,
    PIDT            Idt
);

#endif //__INTERRUPTS_H__
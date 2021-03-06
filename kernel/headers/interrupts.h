#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include "definitions.h"

#define IDT_CRITICAL_ENTRIES            32
#define IDT_MAX_ENTRIES                 256

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

typedef struct _INT_TRAP_FRAME 
{
	QWORD RAX;
	QWORD RBX;
	QWORD RCX;
	QWORD RDX;
	QWORD RSI;
	QWORD RDI;
	QWORD RBP;
	QWORD R8;
	QWORD R9;
	QWORD R10;
	QWORD R11;
	QWORD R12;
	QWORD R13;
	QWORD R14;
	QWORD R15;
	QWORD RIP;
	QWORD FLAGS;
}INT_TRAP_FRAME, *PINT_TRAP_FRAME;

#pragma pack(pop)

void
IntInitializeIdtEntry(
	PIDT_GATE_ENTRY Entry,
	QWORD           ISR
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

void 
IntDumpTrapFrame(
	PINT_TRAP_FRAME TrapFrame
);

#endif //__INTERRUPTS_H__
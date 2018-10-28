#include "interrupts.h"
#include "asm_definitions.h"

void
IntInitializeIdtEntry(
    PIDT_GATE_ENTRY Entry,
    QWORD           Offset,
    WORD            Selector,
    BYTE            Flags
)
{
    Entry->Ist        = 0;         // Currently we are not using IST mechanism
    Entry->Reserved   = 0;
    Entry->Flags      = Flags;
    Entry->Selector   = Selector;
    Entry->OffsetLow  = (Offset & 0xFFFF);
    Entry->OffsetMid  = (Offset & 0xFFFF0000) >> 16;
    Entry->OffsetHigh = (Offset & 0xFFFFFFFF00000000) >> 32;
}

void 
IntInitializeIdt(
    PIDT    Idt
)
{
    for(UINT16 i = 0; i < IDT_MAX_ENTRIES; ++i)
    {
        IntInitializeIdtEntry(&Idt->Entries[i], (QWORD)(&IntCommonISR), CODE_SEGMENT_DESCRIPTOR, INTERRUPT_GATE_TYPE);
    }
}

void
IntInitializeIdtDescriptor(
    PIDT_DESCRIPTOR IdtDescriptor,
    PIDT            Idt
)
{
    IdtDescriptor->IdtBase = Idt;
    IdtDescriptor->IdtSize = sizeof(*Idt);

    IntLoadIdt((void*)IdtDescriptor);
}
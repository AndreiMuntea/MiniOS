#include "interrupts.h"
#include "screen.h"
#include "asm_definitions.h"


void
IntInitializeIdtEntry(
    PIDT_GATE_ENTRY Entry,
    QWORD           ISR
)
{
    Entry->Ist        = 0;         // Currently we are not using IST mechanism
    Entry->Reserved   = 0;
    Entry->Flags      = 0x8E;        // P = 1b; DPL = 00b; Reserved = 0b; type = 1110b -> code | conforming | executable and readable | not 
    Entry->Selector   = 0x8;
    Entry->OffsetLow  = (ISR & 0xFFFF);
    Entry->OffsetMid  = (ISR & 0xFFFF0000) >> 16;
    Entry->OffsetHigh = (ISR & 0xFFFFFFFF00000000) >> 32;
}

void 
IntInitializeIdt(
    PIDT    Idt
)
{
    UINT16 i = 0;

    for(; i < IDT_CRITICAL_ENTRIES; ++i)
    {
        IntInitializeIdtEntry(&(Idt->Entries[i]), (QWORD)(IntCriticalISR));
    }

    for(; i < IDT_MAX_ENTRIES; ++i)
    {
        IntInitializeIdtEntry(&(Idt->Entries[i]), (QWORD)(IntCommonISR));
    }

    IntInitializeIdtEntry(&(Idt->Entries[33]), (QWORD)IntKeyboardISR);
}

void
IntInitializeIdtDescriptor(
    PIDT_DESCRIPTOR IdtDescriptor,
    PIDT            Idt
)
{
    IdtDescriptor->IdtBase = Idt;
    IdtDescriptor->IdtSize = sizeof(*Idt) - 1;

    IntLoadIdt((void*)IdtDescriptor);
}
 
void 
IntDumpTrapFrame(
    PINT_TRAP_FRAME TrapFrame
)
{
    ScClearScreen();
    ScPrint("ENCOUNTERED AN ERROR %n");
    ScPrint("RAX = 0x%x    RBX   = 0x%x %n", TrapFrame->RAX, TrapFrame->RBX);
    ScPrint("RCX = 0x%x    RDX   = 0x%x %n", TrapFrame->RCX, TrapFrame->RDX);
    ScPrint("RSI = 0x%x    RDI   = 0x%x %n", TrapFrame->RSI, TrapFrame->RDI);
    ScPrint("R8  = 0x%x    R9    = 0x%x %n", TrapFrame->R8,  TrapFrame->R9);
    ScPrint("R10 = 0x%x    R11   = 0x%x %n", TrapFrame->R10, TrapFrame->R11);
    ScPrint("R12 = 0x%x    R13   = 0x%x %n", TrapFrame->R12, TrapFrame->R13);
    ScPrint("R14 = 0x%x    R15   = 0x%x %n", TrapFrame->R14, TrapFrame->R15);
    ScPrint("RBP = 0x%x    FLAGS = 0x%x %n", TrapFrame->RBP, TrapFrame->FLAGS);
    ScPrint("RIP = 0x%x %n", TrapFrame->RIP);
}

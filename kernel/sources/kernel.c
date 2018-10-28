#include "global.h"
#include "screen.h"
#include "asm_definitions.h"

int main(void)
{
    InitGlobalData();

    ScClearScreen();
    ScPrint("Sizeof: %x ", sizeof(BYTE));
    ScPrint("Sizeof: %x ", sizeof(WORD));
    ScPrint("Sizeof: %x ", sizeof(DWORD));
    ScPrint("Sizeof: %x ", sizeof(IDT_GATE_ENTRY));
    ScPrint("All set!");

    DebugBreak();
    for(;;) {
        __asm__("hlt");
    }
    
    UninitGlobalData();
    return 0;
}
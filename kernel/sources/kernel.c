#include "global.h"
#include "screen.h"
#include "asm_definitions.h"

int main(void)
{
    InitGlobalData();

    ScClearScreen();
    ScPrint("Sizeof: %x ", sizeof(IDT_GATE_ENTRY));
    ScPrint("All set!");

    DebugBreak();
    while(1)
    {

    }
    
    UninitGlobalData();
    return 0;
}
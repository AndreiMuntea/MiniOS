#include "global.h"
#include "screen.h"
#include "asm_definitions.h"

int main(void)
{
    InitGlobalData();

    ScClearScreen();
    ScPrint("All set!");

    DebugBreak();
    for(;;) 
    {
        __asm__("hlt");
    }
    
    UninitGlobalData();
    return 0;
}
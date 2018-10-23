#include "global.h"
#include "screen.h"
#include "asm_definitions.h"

int main(void)
{
    InitGlobalData();

    ScClearScreen();

    DebugBreak();
    for(int i = 0; i < 20; ++i)
    {
        ScPrintNumber(i);
        ScPrintChar(' ');
    }
    DebugBreak();

    UninitGlobalData();
    return 0;
}
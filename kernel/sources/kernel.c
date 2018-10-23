#include "global.h"
#include "screen.h"
#include "asm_definitions.h"

int main(void)
{
    InitGlobalData();

    ClearScreen();

    DebugBreak();
    for(int i = 0; i < 20; ++i)
    {
        PutNumber(i);
        PutChar(' ');
    }
    DebugBreak();

    UninitGlobalData();
    return 0;
}
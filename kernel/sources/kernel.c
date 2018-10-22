#include "global.h"
#include "screen.h"
#include "asm_definitions.h"

int main(void)
{
    InitGlobalData();

    ClearScreen();

    DebugBreak();
    for(int i = 0; i < 2; ++i)
    {
        PutChar('q');
    }
    DebugBreak();

    UninitGlobalData();
    return 0;
}
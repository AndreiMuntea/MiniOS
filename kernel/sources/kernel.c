#include "global.h"
#include "screen.h"
#include "asm_definitions.h"

int main(void)
{
    InitGlobalData();

    ClearScreen();

    DebugBreak();
    PutString("Ana are mere Ana are mere", sizeof("Ana are mere Ana are mere"));

    DebugBreak();

    UninitGlobalData();
    return 0;
}
#include "global.h"
#include "screen.h"
#include "asm_definitions.h"

int main(void)
{
    InitGlobalData();

    ScClearScreen();
    ScPrint("All set!%n");

    TestTrapFrame();
    Halt();
    
    UninitGlobalData();
    return 0;
}
#include "global.h"
#include "screen.h"
#include "asm_definitions.h"

int main(void)
{
    InitGlobalData();

    ScClearScreen();
    
    //TestTrapFrame();
    Halt();
    
    UninitGlobalData();
    return 0;
}
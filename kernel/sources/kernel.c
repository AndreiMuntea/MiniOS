#include "global.h"
#include "screen.h"
#include "asm_definitions.h"

int main(void)
{
    InitGlobalData();

    ScClearScreen();
    ScPrint("All set!%n");

    ScPrint("Ana are mere!%n");
    ScPrint("0x%x 0x%x 0x%x%n", (QWORD)10, (QWORD)20, (QWORD)30);
    ScPrint("Ana are mere!%n");

    Halt();
    
    UninitGlobalData();
    return 0;
}
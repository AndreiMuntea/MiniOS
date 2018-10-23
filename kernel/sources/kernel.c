#include "global.h"
#include "screen.h"
#include "asm_definitions.h"

int main(void)
{
    InitGlobalData();

    ScClearScreen();

    DebugBreak();
    char ana[] = "Ana are";
    ScPrint("Afisam %s cu %d cu ahahah %s", ana, (QWORD)(5239), ana);
    DebugBreak();

    UninitGlobalData();
    return 0;
}
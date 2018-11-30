#include "global.h"
#include "asm_definitions.h"
#include "console.h"


int main(void)
{
    InitGlobalData();
    MemInit();

    ScClearScreen();
    DebugBreak();
    char* page = (char*)MemAllocPage();
    for(int i = 'a', j = 0 ; i < 'z'; ++i, ++j)
    {
        page[j] = i;
    }
    DebugBreak();
    // while(TRUE)
    // {
    //     ConsoleReadCommand();
    //     ConsoleMatchCommand();
    // }


    Halt();
    
    MemUninit();
    UninitGlobalData();
    return 0;
}

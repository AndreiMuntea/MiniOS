#include "global.h"
#include "screen.h"
#include "asm_definitions.h"

void TestTimer();

int main(void)
{
    InitGlobalData();

    ScClearScreen();
    
    //TestTrapFrame();
    TestTimer();

    Halt();
    
    UninitGlobalData();
    return 0;
}



void TestTimer()
{
    ScPrint("Now we are sleeping 3 seconds...%n");
    TimerSleep(3000);
    ScPrint("Done sleeping!%n");
}
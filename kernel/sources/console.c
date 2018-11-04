#include "console.h"
#include "screen.h"
#include "global.h"
#include "utils.h"
#include "asm_definitions.h"

void 
ConsolePrintHelp(void)
{
    ScPrint("Available commands:%n");
    ScPrint("    > trapframe - Generates a division by 0 exception:%n");
    ScPrint("    > timer     - Prints 2 consecutive delayed messages:%n");
}

void
ConsoleReadCommand(void)
{
    ScPrint("Your command: ");
    while(!gGlobalData.KeyboardData.BufferCompleted)
    {
        TimerSleep(500);
    }
}

void
ConsoleMatchCommand(void)
{
    if (UtilsAreStringsEqual("trapframe", sizeof("trapframe"), gGlobalData.KeyboardData.Buffer, gGlobalData.KeyboardData.BufferSize))
    {
        ScPrint("WOOOHOOO11111");
    }
    else if (UtilsAreStringsEqual("timer", sizeof("timer"), gGlobalData.KeyboardData.Buffer, gGlobalData.KeyboardData.BufferSize))
    {
        ScPrint("WOOHOOOO2222");
    }
    else
    {
        ScClearScreen();
        ScPrint("Invalid command!%n");
        ConsolePrintHelp();
    }
}
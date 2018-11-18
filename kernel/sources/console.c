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
    ScPrint("    > readdisk  - Reads last sector from disk. Special sector:%n");
}

void
ConsoleReadCommand(void)
{
    ScClearScreen();
    ConsolePrintHelp();
    ScPrint("Your command: ");

    ConsoleResetCommandBuffer();
    ConsoleSignalStartOfCommand();
    while(ConsoleIsReceivingInput());
    
}

void
ConsoleMatchCommand(void)
{
    ScClearScreen();
    if (UtilsAreStringsEqual("trapframe", sizeof("trapframe"), gGlobalData.ConsoleData.CommandBuffer, gGlobalData.ConsoleData.CommandBufferCursor))
    {
        ConsoleTrapFrameCommand();
    }
    else if (UtilsAreStringsEqual("timer", sizeof("timer"), gGlobalData.ConsoleData.CommandBuffer, gGlobalData.ConsoleData.CommandBufferCursor))
    {
        ConsoleTimerCommand();
    }
    else if (UtilsAreStringsEqual("readdisk", sizeof("readdisk"), gGlobalData.ConsoleData.CommandBuffer, gGlobalData.ConsoleData.CommandBufferCursor))
    {
        ConsoleReadDiskCommand();
    }
    else
    {
        ScPrint("Invalid command!%n");
        ConsolePrintHelp();
        TimerSleep(1000);
    }
}

void 
ConsoleResetCommandBuffer()
{
    gGlobalData.ConsoleData.IsReceivingInput    = FALSE;
    gGlobalData.ConsoleData.CommandBufferCursor = 0;
    UtilsZeroMemory(gGlobalData.ConsoleData.CommandBuffer, sizeof(gGlobalData.ConsoleData.CommandBuffer));
}


void
ConsolePutCharCommandBuffer(
    char Character
)
{
    if (gGlobalData.ConsoleData.CommandBufferCursor == sizeof(gGlobalData.ConsoleData.CommandBuffer))
    {
        return;
    }

    gGlobalData.ConsoleData.CommandBuffer[gGlobalData.ConsoleData.CommandBufferCursor++] = Character;
    ScPrintChar(Character);
}

void
ConsoleEraseLastCharacterCommandBuffer()
{
    if (gGlobalData.ConsoleData.CommandBufferCursor == 0)
    {
        return;
    }
    
    gGlobalData.ConsoleData.CommandBuffer[--gGlobalData.ConsoleData.CommandBufferCursor] = '\0';
    ScEraseChar();
}


BOOLEAN
ConsoleIsReceivingInput()
{
    return gGlobalData.ConsoleData.IsReceivingInput;
}


void
ConsoleSignalEndOfCommand()
{
    ConsolePutCharCommandBuffer(0);
    gGlobalData.ConsoleData.IsReceivingInput = FALSE;
}


void
ConsoleSignalStartOfCommand()
{
    gGlobalData.ConsoleData.IsReceivingInput = TRUE;   
}

void 
ConsoleTrapFrameCommand()
{
    TestTrapFrame();
}

void
ConsoleTimerCommand()
{
    ScPrint("Now we are sleeping 3 seconds...%n");
    TimerSleep(3000);
    ScPrint("Done sleeping!%n");
    TimerSleep(1000);
}

void
ConsoleReadDiskCommand()
{
    // cylinders=0..127, heads=0..15, sectors=1..32

    char  buffer[512] = {0};
    short cylinder    = 127;
    char  sectorIndex = 32;
    char  head        = 15;

    DiskReadSector(cylinder, sectorIndex, head, buffer);

    for(int k = 0; k < sizeof(buffer); ++k)
    {
        ScPrintChar((BYTE)(buffer[k]));
    }
    TimerSleep(5000);
    ScClearScreen();

}
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "definitions.h"
#include "screen.h"

#pragma pack(push, 1)
typedef struct _CONSOLE_DATA
{
    BOOLEAN IsReceivingInput;
    UINT16  CommandBufferCursor;
    char    CommandBuffer[MAX_OFFSET];
}CONSOLE_DATA, *PCONSOLE_DATA;
#pragma pack(pop)

void 
ConsolePrintHelp(void);

void
ConsoleReadCommand(void);

void
ConsoleMatchCommand(void);

void 
ConsoleResetCommandBuffer();

void
ConsoleEraseLastCharacterCommandBuffer();

void
ConsoleSignalEndOfCommand();

void
ConsoleSignalStartOfCommand();

void 
ConsoleTrapFrameCommand();

void
ConsoleTimerCommand();

void
ConsoleReadDiskCommand();

void
ConsoleWriteDiskCommand();

BOOLEAN
ConsoleIsReceivingInput();

void
ConsolePutCharCommandBuffer(
    char Character
);

#endif //__CONSOLE_H__
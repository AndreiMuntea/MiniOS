#include "console.h"
#include "screen.h"
#include "global.h"
#include "utils.h"
#include "asm_definitions.h"

void 
ConsolePrintHelp(void)
{
	ScPrint("Available commands:%n");
	ScPrint("    > trapframe      - Generates a division by 0 exception%n");
	ScPrint("    > timer          - Prints 2 consecutive delayed messages%n");
	ScPrint("    > writedisk      - Writes last sector from disk%n");
	ScPrint("    > readdisk       - Reads last sector from disk%n");
	ScPrint("    > heapscenario1  - Alloc first 3 pages, writes 0xDEADCODEDEADCODE to first qword then frees the memory%n");
	ScPrint("    > heapscenario2  - Alloc 64 pages, frees one then allocate another one%n");
	ScPrint("    > heapscenario3  - Alloc one page, frees it then tries to read from it%n");
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
	else if (UtilsAreStringsEqual("writedisk", sizeof("writedisk"), gGlobalData.ConsoleData.CommandBuffer, gGlobalData.ConsoleData.CommandBufferCursor))
	{
		ConsoleWriteDiskCommand();
	}
	else if (UtilsAreStringsEqual("readdisk", sizeof("readdisk"), gGlobalData.ConsoleData.CommandBuffer, gGlobalData.ConsoleData.CommandBufferCursor))
	{
		ConsoleReadDiskCommand();
	}
	else if (UtilsAreStringsEqual("heapscenario1", sizeof("heapscenario1"), gGlobalData.ConsoleData.CommandBuffer, gGlobalData.ConsoleData.CommandBufferCursor))
	{
		ConsoleHeapScenario1Command();
	}
	else if (UtilsAreStringsEqual("heapscenario2", sizeof("heapscenario2"), gGlobalData.ConsoleData.CommandBuffer, gGlobalData.ConsoleData.CommandBufferCursor))
	{
		ConsoleHeapScenario2Command();
	}
	else if (UtilsAreStringsEqual("heapscenario3", sizeof("heapscenario3"), gGlobalData.ConsoleData.CommandBuffer, gGlobalData.ConsoleData.CommandBufferCursor))
	{
		ConsoleHeapScenario3Command();
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

void
ConsoleWriteDiskCommand()
{
	// cylinders=0..127, heads=0..15, sectors=1..32

	char  buffer[512] = {"THIS IS THE LAST SECTOR! I DID IT! WOOOHOOOOO! FILLING WITH RANDOM CHARACTERS! BLA BLA BLA BLA"};
	short cylinder    = 127;
	char  sectorIndex = 32;
	char  head        = 15;

	DiskWriteSector(cylinder, sectorIndex, head, buffer);
}

void
ConsoleHeapScenario1Command()
{
	QWORD* p1 = (QWORD*)HeapAlloc(&gGlobalData.Heap);
	QWORD* p2 = (QWORD*)HeapAlloc(&gGlobalData.Heap);
	QWORD* p3 = (QWORD*)HeapAlloc(&gGlobalData.Heap);

	p1[0] = 0xDEADC0DEDEADC0DE;
	p2[0] = 0xDEADC0DEDEADC0DE;
	p3[0] = 0xDEADC0DEDEADC0DE;

	ScPrint("Allocating 1st page : %x%n", p1);
	ScPrint("Allocating 2nd page : %x%n", p2);
	ScPrint("Allocating 3rd page : %x%n", p3);
 
	DebugBreak();

	HeapFree(&gGlobalData.Heap, p1);
	HeapFree(&gGlobalData.Heap, p2);
	HeapFree(&gGlobalData.Heap, p3);
}

void
ConsoleHeapScenario2Command()
{
	QWORD* pages[MAX_PAGES + 1] = {0};
	for(BYTE i = 0; i < MAX_PAGES; ++i)
	{
		pages[i] = (QWORD*)HeapAlloc(&gGlobalData.Heap);
	}

	ScPrint("Allocated 64 pages %n");
	
	pages[MAX_PAGES] = (QWORD*)HeapAlloc(&gGlobalData.Heap);
	ScPrint("Allocating 65th page : %x%n", pages[MAX_PAGES]);

	ScPrint("Freeing 34th page : %x%n", pages[34]);
	HeapFree(&gGlobalData.Heap, pages[34]);

	pages[MAX_PAGES] = (QWORD*)HeapAlloc(&gGlobalData.Heap);
	ScPrint("Allocating 65th page : %x%n", pages[MAX_PAGES]);

	pages[MAX_PAGES][0] = 0xDEADC0DEDEADC0DE;

	for(BYTE i = 0; i < MAX_PAGES + 1; ++i)
	{
		HeapFree(&gGlobalData.Heap, pages[i]);
	}
	DebugBreak();
}

void
ConsoleHeapScenario3Command()
{
	QWORD* p = (QWORD*)HeapAlloc(&gGlobalData.Heap);

	ScPrint("Allocated 1 page %x %n", p);
	p[0] = 0xABCDEF;

	ScPrint("Wrote %x %n", p[0]);

	HeapFree(&gGlobalData.Heap, p);
	
	ScPrint("Trying to read from that page after free%n");
	DebugBreak();

	QWORD dummy = p[0];
	ScPrint("Read: %x %n", dummy);
	DebugBreak();
}
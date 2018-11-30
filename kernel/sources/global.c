#include "global.h"
#include "utils.h"

 GLOBAL_DATA gGlobalData;

void 
InitGlobalData(void)
{
	gGlobalData.ScreenData.VideoMemory   = (PSCREEN)(0x000B8000);
	gGlobalData.ScreenData.CurrentColumn = 0;
	gGlobalData.ScreenData.CurrentLine   = 0;
	gGlobalData.ScreenData.Color         = 0x07;    // lightgrey on black - DOS default

	IntInitializeIdt(&gGlobalData.Idt);
	IntInitializeIdtDescriptor(&gGlobalData.IdtDescriptor, &gGlobalData.Idt);

	ConsoleResetCommandBuffer();
}

void
UninitGlobalData(void)
{
	gGlobalData.ScreenData.VideoMemory   = NULL;
	gGlobalData.ScreenData.CurrentColumn = 0;
	gGlobalData.ScreenData.CurrentLine   = 0;
	gGlobalData.ScreenData.Color         = 0;
	
	ConsoleResetCommandBuffer();
}

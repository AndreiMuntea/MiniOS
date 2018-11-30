#include "global.h"
#include "asm_definitions.h"
#include "console.h"


int main(void)
{
	InitGlobalData();
	MemInit();

	ScClearScreen();
	DebugBreak();
	char* page[65] = {0};
	for(int k = 0; k < 64; ++k)
	{
		page[k] = MemAllocPage();
	}
	page[64] = MemAllocPage();
	if(page[64] == NULL)
	{
		ScPrint("64th page is null %n");
	}

	ScPrint("%x%n", page[33]);
	MemFreePage(33);
	page[64] = MemAllocPage();
	ScPrint("%x%n", page[64]);
	MemFreePage(64);

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

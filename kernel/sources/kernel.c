#include "global.h"
#include "asm_definitions.h"
#include "console.h"


int main(void)
{
	InitGlobalData();

	while(TRUE)
	{
	    ConsoleReadCommand();
	    ConsoleMatchCommand();
	}

	Halt();

	UninitGlobalData();
	return 0;
}

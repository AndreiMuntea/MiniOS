#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "definitions.h"

#define MAX_LINES       25
#define MAX_COLUMNS     80
#define MAX_OFFSET      25 * 80

#pragma pack(push, 1)
typedef struct _SCREEN
{
	char Character;
	BYTE Color;
}SCREEN, *PSCREEN;

typedef struct _SCREEN_DATA
{
	PSCREEN VideoMemory;
	BYTE    Color;
	BYTE    CurrentLine;
	BYTE    CurrentColumn;
}SCREEN_DATA, *PSCREEN_DATA;
#pragma pack(pop)


void 
ScClearScreen(void);

void
ScPrintString(
	char*   String
);

void
ScPrintChar(
	char Character
);

void
ScEraseChar();

void
ScPrintNumber(
	QWORD Number
);

void
ScPrintNewLine();

void
ScPrint(
	char*   Format,
	...
);

#endif //__SCREEN_H__
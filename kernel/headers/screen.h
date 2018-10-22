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
    UINT8   CurrentRow;
    UINT8   CurrentColumn;
    UINT16  CurrentOffset;
}SCREEN_DATA, *PSCREEN_DATA;
#pragma pack(pop)


void 
ClearScreen(void);

void
PutString(
    char*   String,
    UINT16  StringSize
);

void
PutChar(
    char Character
);

#endif //__SCREEN_H__
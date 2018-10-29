#include "screen.h"
#include "global.h"
#include "utils.h"

void 
ScClearScreen(void)
{
    for(int i = 0; i < MAX_OFFSET; ++i)
    {
        gGlobalData.ScreenData.VideoMemory[i].Character = ' ';
        gGlobalData.ScreenData.VideoMemory[i].Color = 0;
    }

    gGlobalData.ScreenData.CurrentColumn = 0;
    gGlobalData.ScreenData.CurrentLine = 0;
}

void
ScPrintChar(
    char Character
)
{
    BYTE currentLine    = gGlobalData.ScreenData.CurrentLine;
    BYTE currentColumn  = gGlobalData.ScreenData.CurrentColumn;
    BYTE color          = gGlobalData.ScreenData.Color;
    UINT16 offset       = currentLine * MAX_COLUMNS + currentColumn;

    gGlobalData.ScreenData.VideoMemory[offset].Character = Character;
    gGlobalData.ScreenData.VideoMemory[offset].Color     = color;
    
    currentColumn++;
    gGlobalData.ScreenData.CurrentLine  += (currentColumn / MAX_COLUMNS);
    gGlobalData.ScreenData.CurrentColumn = (currentColumn % MAX_COLUMNS);
}

void
ScPrintString(
    char*   String
)
{
    for(; *String != 0; ++String)
    {
        ScPrintChar(*String);
    }
}

void
ScPrintNumber(
    QWORD Number
)
{
    char result[19] = {0};
    BYTE rem = 0;
    BYTE size = 0;

    if (Number == 0)
    {
        ScPrintChar('0');
        return;
    }

    while (Number != 0)
    {
        rem = Number % 16;
        Number = Number / 16;
        result[size++] = UtilsHexdigitToChar(rem); 
    }

    for(BYTE i = 0, j = size - 1; i < j; ++i, --j)
    {
        UtilsSwapChars(&result[i], &result[j]);
    }

    ScPrintString(result);
}


void 
ScPrintNewLine(void)
{
    gGlobalData.ScreenData.CurrentLine++;
    gGlobalData.ScreenData.CurrentColumn = 0;
}

void
ScPrint(
    char*   Format,
    ...
)
{
    BYTE argumentIndex = 0;

    while(*Format != 0)
    {
        char currentChar = *Format++;
        if (currentChar != '%')
        {
            ScPrintChar(currentChar);
            continue;
        }

        char type = *Format++;
        void* argument = (void*)((BYTE*)(&Format) + ++argumentIndex * sizeof(void*));

        switch (type)
        {
        case 'x':
            ScPrintNumber(*(QWORD*)argument);
            break;
        case 's':
            ScPrintString(*(char**)argument);
            break;
        case 'n':
            ScPrintNewLine();
            break;
        }
        
    }
}
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
    WORD offset         = (WORD)currentLine * (WORD)MAX_COLUMNS + (WORD)currentColumn;

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
    char result[17] = {0};
    BYTE rem = 0;
    BYTE size = 16;

    for (BYTE i = 0; i < size; ++i)
    {
        result[i] = '0';
    }

    while (Number != 0)
    {
        rem = Number % 16;
        Number = Number / 16;
        result[--size] = UtilsHexdigitToChar(rem); 
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
    void* argument = (void*)(&Format);

    while(*Format != 0)
    {
        char currentChar = *Format++;
        if (currentChar != '%')
        {
            ScPrintChar(currentChar);
            continue;
        }

        char type = *Format++;
        argument = (char*)(argument) + sizeof(void*);

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
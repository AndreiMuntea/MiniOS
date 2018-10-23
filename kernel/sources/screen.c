#include "screen.h"
#include "global.h"

void 
ClearScreen(void)
{
    for(int i = 0; i < MAX_OFFSET; ++i)
    {
        gGlobalData.ScreenData.VideoMemory[i].Character = ' ';
        gGlobalData.ScreenData.VideoMemory[i].Color = 0;
    }
    gGlobalData.ScreenData.CurrentOffset = 0;
}

void
PutChar(
    char Character
)
{
    UINT16 offset = gGlobalData.ScreenData.CurrentOffset;
    BYTE color    = gGlobalData.ScreenData.Color;

    gGlobalData.ScreenData.VideoMemory[offset].Character = Character;
    gGlobalData.ScreenData.VideoMemory[offset].Color     = color;
    gGlobalData.ScreenData.CurrentOffset++;
}

void
PutString(
    char*   String,
    UINT16  StringSize
)
{
    for(UINT16 i = 0; i < StringSize; ++i)
    {
        PutChar(String[i]);
    }
}

void
PutNumber(
    QWORD Number
)
{
    char result[18] = {0};
    BYTE rem = 0;
    BYTE size = 0;

    if (Number == 0)
    {
        PutString("0x0", sizeof("0x0"));
        return;
    }

    for(int i = 0; i < sizeof(result); ++i)
    {
        result[i] = '0';
    }

    while (Number != 0)
    {
        rem = Number % 16;
        Number = Number / 16;
        result[size++] = (rem < 10) ?  '0' + rem
                                    :  'A' + rem - 10;  
    }

    size += 2;  // for 0x
    for(BYTE i = 0, j = size - 1; i < j; ++i, --j)
    {
        char aux = result[i];
        result[i] = result[j];
        result[j] = aux;
    }

    result[1] = 'x';
    PutString(result, size);
}
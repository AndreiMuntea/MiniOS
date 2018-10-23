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
#include "global.h"

 GLOBAL_DATA gGlobalData;

void 
InitGlobalData(void)
{
    gGlobalData.ScreenData.VideoMemory   = (PSCREEN)(0x000B8000);
    gGlobalData.ScreenData.CurrentOffset = 0;
    gGlobalData.ScreenData.Color         = 0x07;    // lightgrey on black - DOS default
}

void
UninitGlobalData(void)
{
    gGlobalData.ScreenData.VideoMemory   = NULL;
    gGlobalData.ScreenData.CurrentOffset = 0;
    gGlobalData.ScreenData.Color         = 0;
}

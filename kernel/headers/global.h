#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "definitions.h"
#include "screen.h"

#pragma pack(push, 1)
typedef struct _GLOBAL_DATA
{
    SCREEN_DATA ScreenData;
}GLOBAL_DATA, *PGLOBAL_DATA;
#pragma pack(pop)

extern GLOBAL_DATA gGlobalData;

void 
InitGlobalData(void);

void
UninitGlobalData(void);

#endif //__GLOBAL_H__
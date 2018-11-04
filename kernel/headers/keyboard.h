#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "screen.h"

#pragma pack(push, 1)
typedef struct _KEYBOARD_DATA
{
    char    Buffer[MAX_OFFSET];
    UINT16  BufferSize;
    BOOLEAN BufferCompleted;
}KEYBOARD_DATA, *PKEYBOARD_DATA;
#pragma pack(pop)

void
KeyboardResetKeyboardData(
    PKEYBOARD_DATA KeyboardData
);

void 
KeyboardKeyPressed(
    char Code
);


#endif //__KEYBOARD_H__
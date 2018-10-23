#ifndef __UTILS__
#define __UTILS__

#include "definitions.h"

void 
UtilsSwapBytes(
    BYTE* FirstByte,
    BYTE* SecondByte
);

char
UtilsHexdigitToChar(
    BYTE Hexdigit
);

#endif //__UTILS__
#ifndef __UTILS__
#define __UTILS__

#include "definitions.h"

void 
UtilsSwapBytes(
    BYTE* FirstByte,
    BYTE* SecondByte
);

void 
UtilsSwapChars(
    char* FirstChar,
    char* SecondChar
);

char
UtilsHexdigitToChar(
    BYTE Hexdigit
);

#endif //__UTILS__
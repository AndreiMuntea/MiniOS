#ifndef __UTILS__
#define __UTILS__

#include "definitions.h"

char
UtilsHexdigitToChar(
    BYTE Hexdigit
);

void
UtilsZeroMemory(
    char*   Buffer,
    QWORD   BufferSize
);

BOOLEAN
UtilsAreStringsEqual(
    char*   Buffer1,
    QWORD   Buffer1Size,
    char*   Buffer2,
    QWORD   Buffer2Size
);


#endif //__UTILS__
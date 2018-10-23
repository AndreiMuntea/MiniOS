#include "utils.h"

void 
UtilsSwapBytes(
    BYTE* FirstByte,
    BYTE* SecondByte
)
{
    BYTE aux    = *FirstByte;
    *FirstByte  = *SecondByte;
    *SecondByte = aux;
}

char
UtilsHexdigitToChar(
    BYTE Hexdigit
)
{
    
    return (Hexdigit < 10) ?  '0' + Hexdigit
                           :  'A' + Hexdigit - 10;  
}


#include "utils.h"

char
UtilsHexdigitToChar(
    BYTE Hexdigit
)
{
    return (Hexdigit < 10) ?  '0' + Hexdigit
                           :  'A' + Hexdigit - 10;  
}


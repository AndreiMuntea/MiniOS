#include "keyboard.h"
#include "asm_definitions.h"
#include "screen.h"

void 
KeyboardKeyPressed(char Code)
{
    ScPrintChar(Code);
}
#include "keyboard.h"
#include "asm_definitions.h"
#include "screen.h"

static const char gDigitsMapping[] = "1234567890";
static const char gLettersMapping1[] = "qwertyuiop";
static const char gLettersMapping2[] = "asdfghjkl";
static const char gLettersMapping3[] = "zxcvbnm";


BOOLEAN
KeyboardIsKeyReleased(
    char Code
) 
{
    return (Code & 0x80);
}

void 
KeyboardKeyPressed(
    char Code
)
{
    char character = 0;
    if (KeyboardIsKeyReleased(Code))
    {
        return;
    }
    else
    {
        if (Code >= 0x2 && Code <= 0xB)
        {
           character = gDigitsMapping[Code - 0x2];
        }
        else if (Code >= 0x10 && Code <= 0x19)
        {
            character = gLettersMapping1[Code - 0x10];
        }
        else if (Code >= 0x1E && Code <= 0x26)
        {
            character = gLettersMapping2[Code - 0x1E];   
        }
        else if (Code >= 0x2C && Code <= 0x32)
        {
            character = gLettersMapping3[Code - 0x2C];   
        }
    }

    if (character != 0)
    {
        ScPrintChar(character);
    }
}
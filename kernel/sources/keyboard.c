#include "screen.h"
#include "keyboard.h"
#include "global.h"
#include "utils.h"
#include "asm_definitions.h"

static const char gDigitsMapping[]     = "1234567890";
static const char gLettersMappingQ_P[] = "qwertyuiop";
static const char gLettersMappingA_L[] = "asdfghjkl";
static const char gLettersMappingZ_M[] = "zxcvbnm";


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
            character = gLettersMappingQ_P[Code - 0x10];
        }
        else if (Code >= 0x1E && Code <= 0x26)
        {
            character = gLettersMappingA_L[Code - 0x1E];   
        }
        else if (Code >= 0x2C && Code <= 0x32)
        {
            character = gLettersMappingZ_M[Code - 0x2C];   
        }
        else if (Code == 0x1C) // Enter
        {
            gGlobalData.KeyboardData.BufferCompleted = TRUE;
            gGlobalData.KeyboardData.BufferSize++;
            ScPrintNewLine();
        }
        else if (Code == 0xE) // Backspace
        {
            ScEraseChar();
        }
        else if (Code == 0x39) // Space
        {
            character = ' ';
        }
    }

    if (character != 0)
    {
        gGlobalData.KeyboardData.Buffer[gGlobalData.KeyboardData.BufferSize++] = character;
        ScPrintChar(character);
    }
}

void
KeyboardResetKeyboardData(
    PKEYBOARD_DATA KeyboardData
)
{
    KeyboardData->BufferCompleted = FALSE;
    KeyboardData->BufferSize = 0;
    UtilsZeroMemory((char*)(&KeyboardData->Buffer), sizeof(KeyboardData->Buffer));
}
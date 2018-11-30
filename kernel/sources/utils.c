#include "utils.h"

char
UtilsHexdigitToChar(
	BYTE Hexdigit
)
{
	return (Hexdigit < 10) ?  '0' + Hexdigit
						   :  'A' + Hexdigit - 10;  
}

void
UtilsZeroMemory(
	char*   Buffer,
	QWORD   BufferSize
)
{
	for (QWORD i = 0; i < BufferSize; ++i)
	{
		Buffer[i] = 0;
	}
}

BOOLEAN
UtilsAreStringsEqual(
	char*   Buffer1,
	QWORD   Buffer1Size,
	char*   Buffer2,
	QWORD   Buffer2Size
)
{
	if (Buffer1Size != Buffer2Size)
	{
		return FALSE;
	}

	for(QWORD i = 0; i < Buffer1Size; ++i)
	{
		if (Buffer1[i] != Buffer2[i])
		{
			return FALSE;
		}
	}

	return TRUE;
}
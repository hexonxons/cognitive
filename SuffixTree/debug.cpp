/*
 * ������� ������ ������
 * blacklist (c) 2010
 */

#include "debug.h"

//
// DebugPrint()
// ����� �����-������ � printf-�������
// lpszFormat - ������ �������
//
VOID WINAPI DebugPrint(LPTSTR lpszFormat, ...)
{
    va_list args;

    // ������� ���������
    va_start(args, lpszFormat);

#ifdef _DEBUG

    CHAR szString[4096];

    // ���������� � ������
    wvsprintf(szString, lpszFormat, args);

    // ������� � �����
    OutputDebugString(szString);

#endif // _DEBUG

    va_end(args);
}

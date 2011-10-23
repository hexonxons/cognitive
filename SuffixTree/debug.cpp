/*
 * функция вывода дебага
 * blacklist (c) 2010
 */

#include "debug.h"

//
// DebugPrint()
// вывод дебаг-строки в printf-формате
// lpszFormat - строка формата
//
VOID WINAPI DebugPrint(LPTSTR lpszFormat, ...)
{
    va_list args;

    // получим аргументы
    va_start(args, lpszFormat);

#ifdef _DEBUG

    CHAR szString[4096];

    // напечатаем в строку
    wvsprintf(szString, lpszFormat, args);

    // выведем в дебаг
    OutputDebugString(szString);

#endif // _DEBUG

    va_end(args);
}

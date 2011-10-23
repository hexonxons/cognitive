/*
 * прототипы debug.cpp
 * blacklist (c) 2010
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

VOID WINAPI DebugPrint(LPTSTR lpszFormat, ...);

#endif  //__DEBUG_H__
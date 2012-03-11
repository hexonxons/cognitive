
// NewsColoring.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CNewsColoringApp:
// О реализации данного класса см. NewsColoring.cpp
//

class CNewsColoringApp : public CWinAppEx
{
public:
	CNewsColoringApp();

// Переопределение
	public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CNewsColoringApp theApp;

// NewsColoring.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CNewsColoringApp:
// � ���������� ������� ������ ��. NewsColoring.cpp
//

class CNewsColoringApp : public CWinAppEx
{
public:
	CNewsColoringApp();

// ���������������
	public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CNewsColoringApp theApp;
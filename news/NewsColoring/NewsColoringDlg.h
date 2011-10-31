
// NewsColoringDlg.h : файл заголовка
//

#pragma once
#include "afxcmn.h"


// диалоговое окно CNewsColoringDlg
class CNewsColoringDlg : public CDialog
{
// Создание
public:
	CNewsColoringDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_NEWSCOLORING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
    CRichEditCtrl m_RichCtrl;
public:
    afx_msg void OnBnClickedBtnrun();
private:
    CString m_FileName;
    int m_minFreq;
    int m_minWordlen;
    int m_newsNum;
};

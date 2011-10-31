
// NewsColoringDlg.h : ���� ���������
//

#pragma once
#include "afxcmn.h"


// ���������� ���� CNewsColoringDlg
class CNewsColoringDlg : public CDialog
{
// ��������
public:
	CNewsColoringDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
	enum { IDD = IDD_NEWSCOLORING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
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

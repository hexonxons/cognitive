
// NewsColoringDlg.h : ���� ���������
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


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
    CHARFORMAT2 cfDefault;
    CRichEditCtrl m_RichCtrl;
    CString m_fileName;
    std::string m_fileData;
    int m_minFreq;
    int m_minWordlen;
    int m_newsNum;
    CListBox m_ListBox;
    std::vector<std::vector<std::pair<int, int>>> tagRanges;
    bool m_radioBlue;
    bool m_radioGreen;
    bool m_radioRed;

    void ColorRichText(int start, int end, COLORREF color);
public:
    afx_msg void OnBnClickedBtnrun();
    afx_msg void OnBnClickedRadiored();
    afx_msg void OnBnClickedRadiogreen();
    afx_msg void OnBnClickedRadioblue();
    afx_msg void OnLbnSelchangeList();
private:
    CListBox m_ListRanges;
public:
    afx_msg void OnLbnSelchangeListranges();
    afx_msg void OnBnClickedBtnresetsel();
};


// NewsColoringDlg.h : ���� ���������
//

#pragma once
#include "../newsfinder/utils.h"
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
    // ���� ������ html-������
    CRichEditCtrl m_RichCtrl;
    // ���������� ������ ������ �����
    CComboBox m_SiteSelect;
    // ������ ��������� ������������� ����������������������
    CListBox m_ListFreq;
    // ������ ���� ������ ��������
    CListBox m_ListRanges;
    // ������ ���� ������
    CListBox m_PropList;

    // ������ ������ �����
    bool m_RadioBlue;
    bool m_RadioGreen;
    bool m_RadioRed;
    // ���������� �� ��� ������� � ��������� ������������������
    bool m_IsSingleTagSeq;

    // ��������� � �������� ������, ���������� �������
    int m_NewsBeginNum;
    int m_NewsEndNum;

    std::vector<CTagSequence> m_TagRanges;
    // ���������� ����� ��������
    std::string m_FileData;
    // ������ ������
    CHARFORMAT2 cfDefault;
    // URL ����� ��������
    CString m_SiteURL;

    void Init(std::string, int, int, int);
    void ClearDialog();
    void ColorRichText(int start, int end, COLORREF color);
    // ������� ������������ ����������� ����
    void Rebuild();
public:
    afx_msg void OnBnClickedRadiored();
    afx_msg void OnBnClickedRadiogreen();
    afx_msg void OnBnClickedRadioblue();
    afx_msg void OnLbnSelchangeList();
    afx_msg void OnLbnSelchangeListranges();
    afx_msg void OnBnClickedBtnresetsel();
    afx_msg void OnBnClickedBtnsetbegin();
    afx_msg void OnBnClickedBtnsetend();
    afx_msg void OnBnClickedBtnprintnews();
    afx_msg void OnBnClickedBntsingle();
    afx_msg void OnBnClickedBtncolorall();
    afx_msg void OnBnClickedBtnfind();
    afx_msg void OnCbnSelchangeSiteselect();
};

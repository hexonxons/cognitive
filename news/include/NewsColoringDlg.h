
// NewsColoringDlg.h : ���� ���������
//

#pragma once
#include "utils.h"
#include "afxwin.h"
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
    // ���� ������ html-������
    CRichEditCtrl m_RichCtrl;
    // ���������� ������ ������ �����
    CComboBox m_SiteSelect;
    CComboBox m_FileSelect;

    CListBox m_ListFreq;
    CListBox m_ListRanges;
    CListBox m_PropFreq;
    CListBox m_PropRanges;

    // ������ ������ �����
    bool m_RadioBlue;
    bool m_RadioGreen;
    bool m_RadioRed;
    // ���������� �� ��� ������� � ��������� ������������������
    bool m_IsSingleTagSeq;

    double m_AvgDispersion;

    // ��������� � �������� ������, ���������� �������
    int m_NewsBeginNum;
    int m_NewsEndNum;

    std::vector<CNewsBlock> m_TagRanges;
    std::vector<CNewsBlock> m_DisplayedTagRanges;
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
    void RebuildURL();
    // ������� ������������ ����������� ����
    void RebuildFILE();
    // ������� ���������� ������������������� �� ���������
    void BuildRanges();
    // ������� ��������� �������� ������ � ����������
    void GetNewsFilenames();
    // ��������� �����
    std::vector<CNews> FinishTagsSeq(std::vector<CNews> &);
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
private:
    CSliderCtrl m_SlideDisp;
    CSliderCtrl m_SliderIntersect;
    CSliderCtrl m_SliderHTML;
    CSliderCtrl m_SliderVisibleHTML;

    CEdit m_EditDispersion;
    CEdit m_EditIntersection;
    CEdit m_EditPercToHtml;
    CEdit m_EditPercToVisibleHTML;
public:
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnBnClickedBtnsaveheuristics();
private:
    long m_NewsNumber;
public:
    afx_msg void OnCbnSelchangeFileselect();
    afx_msg void OnEnChangeNewsnumber();
    afx_msg void OnEnKillfocusEditnewsnumber();
};

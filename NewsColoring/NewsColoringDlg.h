
// NewsColoringDlg.h : файл заголовка
//

#pragma once
#include "../newsfinder/utils.h"
#include "afxwin.h"

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
    CComboBox m_SiteSelect;
    CListBox m_ListBox;
    CListBox m_ListRanges;
    CListBox m_propList;

    bool m_radioBlue;
    bool m_radioGreen;
    bool m_radioRed;
    bool isSingleTagSeq;

    int newsBeginNum;
    int newsEndNum;

    std::vector<CTagSequence> tagRanges;
    std::string m_fileData;
    CHARFORMAT2 cfDefault;

    void Init(std::string, int, int, int);
    void ClearDialog();
    void ColorRichText(int start, int end, COLORREF color);
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

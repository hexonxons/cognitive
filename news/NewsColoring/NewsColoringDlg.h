
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
    CHARFORMAT2 cfDefault;
    CRichEditCtrl m_RichCtrl;
    std::string m_fileData;
    CListBox m_ListBox;
    std::vector<std::vector<std::pair<int, int>>> tagRanges;
    bool m_radioBlue;
    bool m_radioGreen;
    bool m_radioRed;
    CListBox m_ListRanges;
    CListBox m_EncodedListLeft;
    CListBox m_EncodedListRight;
    std::vector<CTagDescription> encodedString;
    void ColorRichText(int start, int end, COLORREF color);
    void EncodeString();

    bool isSingleTagSeq;
    int newsBeginNum;
    int newsEndNum;
public:
    afx_msg void OnBnClickedRadiored();
    afx_msg void OnBnClickedRadiogreen();
    afx_msg void OnBnClickedRadioblue();
    afx_msg void OnLbnSelchangeList();
    afx_msg void OnLbnSelchangeListranges();
    afx_msg void OnBnClickedBtnresetsel();
    afx_msg void OnBnClickedGetdecodedLeft();
    afx_msg void OnBnClickedGetdecodedright();
    afx_msg void OnBnClickedBtnsetbegin();
    afx_msg void OnBnClickedBtnsetend();
    afx_msg void OnBnClickedBtnprintnews();
    afx_msg void OnBnClickedBntsingle();
    afx_msg void OnBnClickedBtncolorall();
};

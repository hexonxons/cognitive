
// NewsColoringDlg.h : файл заголовка
//

#pragma once
#include "utils.h"
#include "afxwin.h"
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
    // Окно вывода html-текста
    CRichEditCtrl m_RichCtrl;
    // Выпадающий список выбора сайта
    CComboBox m_SiteSelect;
    CComboBox m_FileSelect;

    CListBox m_ListFreq;
    CListBox m_ListRanges;
    CListBox m_PropFreq;
    CListBox m_PropRanges;

    // Кнопки выбора цвета
    bool m_RadioBlue;
    bool m_RadioGreen;
    bool m_RadioRed;
    // Включается ли вся новость в шаблонную последовательность
    bool m_IsSingleTagSeq;

    double m_AvgDispersion;

    // Начальный и конечный шаблон, содержащий новость
    int m_NewsBeginNum;
    int m_NewsEndNum;

    std::vector<CNewsBlock> m_TagRanges;
    std::vector<CNewsBlock> m_DisplayedTagRanges;
    // Содержание файла новостей
    std::string m_FileData;
    // Формат текста
    CHARFORMAT2 cfDefault;
    // URL сайта новостей
    CString m_SiteURL;
    void Init(std::string, int, int, int);
    void ClearDialog();
    void ColorRichText(int start, int end, COLORREF color);
    // функция перезагрузки диалогового окна
    void RebuildURL();
    // функция перезагрузки диалогового окна
    void RebuildFILE();
    // функция построения последовательностей по эвристике
    void BuildRanges();
    // функция получения названий файлов в директории
    void GetNewsFilenames();
    // достройка тегов
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

// NewsColoringDlg.cpp : файл реализации
//

#include "stdafx.h"

#include <algorithm>
#include <fstream>
#include <string>

#include "NewsColoring.h"
#include "NewsColoringDlg.h"

#include "../newsfinder/tagprocess.h"
#include "../include/http.h"


using std::string;
using std::vector;
using std::pair;
using std::fstream;


#ifdef _DEBUG
    #define new DEBUG_NEW
#endif


// диалоговое окно CNewsColoringDlg
CNewsColoringDlg::CNewsColoringDlg(CWnd* pParent /*=NULL*/)
: CDialog(CNewsColoringDlg::IDD, pParent)
, m_RadioBlue(1)
, m_RadioGreen(0)
, m_RadioRed(0)
, m_NewsBeginNum(-1)
, m_NewsEndNum(-1)
, m_IsSingleTagSeq(false)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    AfxInitRichEdit2();

    cfDefault.dwMask = CFM_CHARSET | CFM_FACE | CFM_SIZE | CFM_OFFSET | CFM_COLOR;
    cfDefault.dwMask ^= CFM_ITALIC ^ CFM_BOLD ^ CFM_STRIKEOUT ^ CFM_UNDERLINE;;
    cfDefault.dwEffects = 0;
    cfDefault.yHeight = 180;
    cfDefault.bCharSet = RUSSIAN_CHARSET;
    cfDefault.bPitchAndFamily = FIXED_PITCH | FF_MODERN;
    cfDefault.yOffset = 0;
    cfDefault.crTextColor = RGB(0,0,0);
    strcpy(cfDefault.szFaceName, "Courier New");
    cfDefault.cbSize = sizeof(cfDefault);
}

void CNewsColoringDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RICHEDIT, m_RichCtrl);
    DDX_Control(pDX, IDC_LISTFREQ, m_ListFreq);
    DDX_Control(pDX, IDC_LISTRANGES, m_ListRanges);
    DDX_Control(pDX, IDC_PROPERTIESLIST, m_PropList);
    DDX_Control(pDX, IDC_SITESELECT, m_SiteSelect);
}

BEGIN_MESSAGE_MAP(CNewsColoringDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_RADIORED, &CNewsColoringDlg::OnBnClickedRadiored)
    ON_BN_CLICKED(IDC_RADIOGREEN, &CNewsColoringDlg::OnBnClickedRadiogreen)
    ON_BN_CLICKED(IDC_RADIOBLUE, &CNewsColoringDlg::OnBnClickedRadioblue)
    ON_LBN_SELCHANGE(IDC_LISTFREQ, &CNewsColoringDlg::OnLbnSelchangeList)
    ON_LBN_SELCHANGE(IDC_LISTRANGES, &CNewsColoringDlg::OnLbnSelchangeListranges)
    ON_BN_CLICKED(IDC_BTNRESETSEL, &CNewsColoringDlg::OnBnClickedBtnresetsel)
    ON_BN_CLICKED(IDC_BTNSETBEGIN, &CNewsColoringDlg::OnBnClickedBtnsetbegin)
    ON_BN_CLICKED(IDC_BTNSETEND, &CNewsColoringDlg::OnBnClickedBtnsetend)
    ON_BN_CLICKED(IDC_BTNPRINTNEWS, &CNewsColoringDlg::OnBnClickedBtnprintnews)
    ON_BN_CLICKED(IDC_BNTSINGLE, &CNewsColoringDlg::OnBnClickedBntsingle)
    ON_BN_CLICKED(IDC_BTNCOLORALL, &CNewsColoringDlg::OnBnClickedBtncolorall)
    ON_BN_CLICKED(IDC_BTNFIND, &CNewsColoringDlg::OnBnClickedBtnfind)
    ON_CBN_SELCHANGE(IDC_SITESELECT, &CNewsColoringDlg::OnCbnSelchangeSiteselect)
END_MESSAGE_MAP()


// обработчики сообщений CNewsColoringDlg
BOOL CNewsColoringDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Задает значок для этого диалогового окна. Среда делает это автоматически,
    //  если главное окно приложения не является диалоговым
    SetIcon(m_hIcon, TRUE);			// Крупный значок
    SetIcon(m_hIcon, FALSE);		// Мелкий значок
    
    // Считываем набор URL-ов новостных сайтов
    fstream FI("sites.cfg", std::ios::in);
    m_SiteSelect.AddString("");
    while (!FI.eof())
    {
        string siteURI = "";
        FI >> siteURI;
        m_SiteSelect.AddString(siteURI.c_str());
    }
    FI.close();
    // Выделяем 1ю строчку
    m_SiteSelect.SetCurSel(0);
    Rebuild();

    return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CNewsColoringDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // контекст устройства для рисования

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Выравнивание значка по центру клиентского прямоугольника
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Нарисуйте значок
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CNewsColoringDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CNewsColoringDlg::Init(string fileData, int minWordlen, int minFreq, int newsNum)
{
    struct sortPredicate
    {
        bool operator () (const CTagSequence &left, const CTagSequence &right)
        {
            return left.tagRange.size() > right.tagRange.size();
        }
    };

    /*std::fstream fileIn("news", std::ios::in);
    m_FileData = std::string((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    CNewsFinder news(m_FileData.c_str(), minWordlen, minFreq);
    fileIn.close();*/

    CNewsFinder news(fileData.c_str(), minWordlen, minFreq);
    m_FileData = fileData;

    vector<pair<string, string>> remDoubleTag;
    vector<string> remTag;
    remDoubleTag.push_back(std::make_pair("<script>", "</script>"));
    remDoubleTag.push_back(std::make_pair("<noscript>", "</noscript>"));
    remDoubleTag.push_back(std::make_pair("<form>", "</form>"));
    remDoubleTag.push_back(std::make_pair("<iframe>", "</iframe>"));
    remDoubleTag.push_back(std::make_pair("<noindex> ", "</noindex> "));
    remDoubleTag.push_back(std::make_pair("<style>", "</style>"));

    remTag.push_back("<html>");
    remTag.push_back("</html>");
    remTag.push_back("<head>");
    remTag.push_back("</head>");
    remTag.push_back("<body>");
    remTag.push_back("</body>");
    remTag.push_back("<link>");
    remTag.push_back("<title>");
    remTag.push_back("</title>");
    remTag.push_back("<meta>");
    remTag.push_back("</meta>");
    remTag.push_back("<base>");
    remTag.push_back("<!-->");
    remTag.push_back("<li>");
    remTag.push_back("</li>");
    remTag.push_back("<p>");
    remTag.push_back("</p>");
    remTag.push_back("<ul>");
    remTag.push_back("</ul>");
    remTag.push_back("<span>");
    remTag.push_back("</span>");

    news.Init(remDoubleTag, remTag);
    news.GetPossibleRanges();
    m_TagRanges = news.GetRanges();

    std::sort(m_TagRanges.begin(), m_TagRanges.end(), sortPredicate());

    m_RichCtrl.HideSelection(FALSE, FALSE);
    m_RichCtrl.SetDefaultCharFormat(cfDefault);
    m_RichCtrl.SetWindowText(m_FileData.c_str());

    for (vector<CTagSequence>::iterator it = m_TagRanges.begin(); it != m_TagRanges.end(); ++it)
    {
        CString str;
        str.AppendFormat(_T("%d - "), it->tagRange.size());
        for (vector<CTagRange>::iterator jt = it->tagRange.begin(); jt != it->tagRange.end(); ++jt)
        {
            str.AppendFormat(_T("(%d,%d) "), jt->begin, jt->end);
        }
        m_ListFreq.AddString(str.GetString());
    }
}

void CNewsColoringDlg::Rebuild()
{
    // Очищаем весь диалог
    ClearDialog();
    if(strlen(m_SiteURL.GetString()) != 0)
    {
        // Получаем html-код страницы
        LPSTR URLContent = GetPageSource(m_SiteURL.GetString());
        Init(URLContent, 4, 8, 0);
    }
}

void CNewsColoringDlg::ClearDialog()
{
    if(!UpdateData(TRUE))
        return;

    // очищаем listbox'ы
    m_ListRanges.ResetContent();
    m_PropList.ResetContent();
    m_ListFreq.ResetContent();

    // кнопки цвета
    m_RadioBlue = false;
    m_RadioGreen = false;
    m_RadioRed = true;

    // убираем текст
    m_RichCtrl.HideSelection(FALSE, FALSE);
    m_RichCtrl.SetWindowText("");

    UpdateData(FALSE);
}

void CNewsColoringDlg::ColorRichText(int start, int end, COLORREF color)
{
    m_RichCtrl.SetRedraw(FALSE);
    CHARFORMAT2 cfNew = cfDefault;
    cfNew.crTextColor = color;

    m_RichCtrl.SetSel(start, end);
    m_RichCtrl.SetSelectionCharFormat(cfNew);
    m_RichCtrl.SetSel(start, start);
    m_RichCtrl.SetRedraw(TRUE);
    m_RichCtrl.RedrawWindow();
}

void CNewsColoringDlg::OnLbnSelchangeList()
{
    if(!UpdateData(TRUE))
        return;

    m_ListRanges.ResetContent();
    int selElem = m_ListFreq.GetCurSel();
    int sz = (m_TagRanges.begin() + selElem)->tagRange.size();

    for (vector<CTagRange>::iterator it = (m_TagRanges.begin() + selElem)->tagRange.begin(); it != (m_TagRanges.begin() + selElem)->tagRange.end(); ++it)
    {
        CString str;
        str.AppendFormat(_T("(%d,%d) , len = %d, perc = %f, perc to vis = %f, intersection = %d"), it->begin, it->end, it->end - it->begin, it->percToHtml, it->percToVisibleHtml, it->innerIntersection);
        m_ListRanges.AddString(str.GetString());
    }
    m_PropList.ResetContent();
    CString format;
    format.AppendFormat(_T("Dispersion : %f"), (m_TagRanges.begin() + selElem)->dispersion);
    m_PropList.AddString(format.GetString());
    format.Empty();
    format.AppendFormat(_T("Perc to html : %f"), (m_TagRanges.begin() + selElem)->percToHtml);
    m_PropList.AddString(format.GetString());
    format.Empty();
    format.AppendFormat(_T("Perc to vis html : %f"), (m_TagRanges.begin() + selElem)->percToVisibleHtml);
    m_PropList.AddString(format.GetString());
    format.Empty();
    format.AppendFormat(_T("Intersection : %d"), (m_TagRanges.begin() + selElem)->innerIntersect);
    m_PropList.AddString(format.GetString());
    UpdateData(FALSE);
}

void CNewsColoringDlg::OnLbnSelchangeListranges()
{
    if(!UpdateData(TRUE))
        return;

    int selElem = m_ListFreq.GetCurSel();
    int selRange = m_ListRanges.GetCurSel();
    COLORREF color = RGB(255 * m_RadioRed, 255 * m_RadioGreen, 255 * m_RadioBlue);
    ColorRichText(((m_TagRanges.begin() + selElem)->tagRange.begin() + selRange)->begin, ((m_TagRanges.begin() + selElem)->tagRange.begin() + selRange)->end + 1, color);

    UpdateData(FALSE);
}

// Снятие выделений с текста
void CNewsColoringDlg::OnBnClickedBtnresetsel()
{    
    if(!UpdateData(TRUE))
        return;
    cfDefault.crTextColor = RGB(0,0,0);
    m_RichCtrl.SetWindowText(m_FileData.c_str());
    UpdateData(FALSE);
}

void CNewsColoringDlg::OnBnClickedBtnsetbegin()
{
    if(!UpdateData(TRUE))
        return;

    m_NewsBeginNum = m_ListFreq.GetCurSel();

    UpdateData(FALSE);
}

void CNewsColoringDlg::OnBnClickedBtnsetend()
{
    if(!UpdateData(TRUE))
        return;

    m_NewsEndNum = m_ListFreq.GetCurSel();

    UpdateData(FALSE);
}

void CNewsColoringDlg::OnBnClickedBtnprintnews()
{
    ///< Выходной файл.
    std::fstream fileOut("out.html", std::ios::out);

    fileOut << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>\n<body>\n";

    unsigned int offset = 0;
    // Получаем строчку новости и выводим её
    for(int i = 0; i < (m_TagRanges.begin() + m_NewsBeginNum)->tagRange.size(); ++i)
    {
        string res = "";
        if (!m_IsSingleTagSeq)
        {
            res = string(m_FileData, ((m_TagRanges.begin() + m_NewsBeginNum)->tagRange.begin() +  i)->begin,
                                     ((m_TagRanges.begin() + m_NewsEndNum)->tagRange.begin() +  i)->end - ((m_TagRanges.begin() + m_NewsBeginNum)->tagRange.begin() +  i)->begin + 1);
        }
        else
        {
            res = string(m_FileData, ((m_TagRanges.begin() + m_NewsBeginNum)->tagRange.begin() +  i)->begin,
                                     ((m_TagRanges.begin() + m_NewsBeginNum)->tagRange.begin() +  i)->end - ((m_TagRanges.begin() + m_NewsBeginNum)->tagRange.begin() +  i)->begin + 1);
        }
        fileOut << res;
        fileOut << "\n<br>#########################################################################################\n";
    }
    fileOut << "\n</body></html>";
}

void CNewsColoringDlg::OnBnClickedBntsingle()
{
    if(!UpdateData(TRUE))
        return;

    m_IsSingleTagSeq = true;
    m_NewsBeginNum = m_ListFreq.GetCurSel();

    UpdateData(FALSE);
}

void CNewsColoringDlg::OnBnClickedBtncolorall()
{
    if(!UpdateData(TRUE))
        return;

    int selElem = m_ListFreq.GetCurSel();
    int sz = (m_TagRanges.begin() + selElem)->tagRange.size();
    COLORREF color = RGB(255 * m_RadioRed, 255 * m_RadioGreen, 255 * m_RadioBlue);

    for (vector<CTagRange>::iterator it = (m_TagRanges.begin() + selElem)->tagRange.begin(); it != (m_TagRanges.begin() + selElem)->tagRange.end(); ++it)
    {
        ColorRichText(it->begin, it->end + 1, color);
    }

    UpdateData(FALSE);
}

void CNewsColoringDlg::OnBnClickedBtnfind()
{
    if(!UpdateData(TRUE))
        return;

    CHARRANGE cr;
    m_RichCtrl.GetSel(cr);
    int begin = cr.cpMin < cr.cpMax ? cr.cpMin : cr.cpMax;
    int end = cr.cpMin > cr.cpMax ? cr.cpMin - 1 : cr.cpMax - 1;
    int ind = 0;
    for (vector<CTagSequence>::iterator it = m_TagRanges.begin(); it < m_TagRanges.end(); ++it)
    {
        ind++;
        for (vector<CTagRange>::iterator jt = it->tagRange.begin(); jt < it->tagRange.end(); ++jt)
        {
            if (jt->begin == begin && jt->end == end)
            {
                m_ListFreq.SetCurSel(ind - 1);
                it = m_TagRanges.end() - 1;
                break;
            }

            if (jt->begin > begin)
            {
                break;
            }
        }
    }
    m_ListFreq.SetSel(-1, false);
    UpdateData(FALSE);
}

// получение URL из выпадающего списка
void CNewsColoringDlg::OnCbnSelchangeSiteselect()
{
    m_SiteURL.ReleaseBuffer();
    int num = m_SiteSelect.GetCurSel();
    int sz = m_SiteSelect.GetLBTextLen(num);
    m_SiteSelect.GetLBText(num, m_SiteURL.GetBuffer(sz));
    Rebuild();
}

// обработчики выбора цвета раскраски текста
void CNewsColoringDlg::OnBnClickedRadiored()
{
    m_RadioBlue = false;
    m_RadioGreen = false;
    m_RadioRed = true;
}

void CNewsColoringDlg::OnBnClickedRadiogreen()
{
    m_RadioBlue = false;
    m_RadioRed = false;
    m_RadioGreen = true;
}

void CNewsColoringDlg::OnBnClickedRadioblue()
{
    m_RadioBlue = true;
    m_RadioRed = false;
    m_RadioGreen = false;
}
// NewsColoringDlg.cpp : файл реализации
//

#include "stdafx.h"

#include <algorithm>
#include <fstream>
#include <string>

#include "NewsColoring.h"
#include "NewsColoringDlg.h"

#include "tagprocess.h"
#include "http.h"


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
    DDX_Control(pDX, IDC_LISTPROPFREQ, m_PropFreq);
    DDX_Control(pDX, IDC_SITESELECT, m_SiteSelect);
    DDX_Control(pDX, IDC_LISTPROPRANGES, m_PropRanges);
    DDX_Control(pDX, IDC_SLIDERDISP, m_SlideDisp);
    DDX_Control(pDX, IDC_SLIDERINTERSECT, m_SliderIntersect);
    DDX_Control(pDX, IDC_SLIDERHTML, m_SliderHTML);
    DDX_Control(pDX, IDC_SLIDERVISIBLEHTML, m_SliderVisibleHTML);
    DDX_Control(pDX, IDC_EDITDISP, m_EditDispersion);
    DDX_Control(pDX, IDC_EDITINTERSECT, m_EditIntersection);
    DDX_Control(pDX, IDC_EDITHTML, m_EditPercToHtml);
    DDX_Control(pDX, IDC_EDITVISHTML, m_EditPercToVisibleHTML);
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
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_BTNSAVEHEURISTICS, &CNewsColoringDlg::OnBnClickedBtnsaveheuristics)
END_MESSAGE_MAP()


// обработчики сообщений CNewsColoringDlg
BOOL CNewsColoringDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Задает значок для этого диалогового окна. Среда делает это автоматически,
    //  если главное окно приложения не является диалоговым
    SetIcon(m_hIcon, TRUE);			// Крупный значок
    SetIcon(m_hIcon, FALSE);		// Мелкий значок
    
    m_SliderHTML.SetRange(0, 100, 1);
    m_SliderVisibleHTML.SetRange(0, 100, 1);
    m_SliderIntersect.SetRange(0, 150000, 1);
    m_SlideDisp.SetRange(0, 1000, 1);

    // Считываем набор URL-ов новостных сайтов
    fstream fSites("config/sites.cfg", std::ios::in);
    m_SiteSelect.AddString("");
    while (!fSites.eof())
    {
        string siteURI = "";
        fSites >> siteURI;
        m_SiteSelect.AddString(siteURI.c_str());
    }
    fSites.close();

    // считываем последнюю эвристику
    fstream fHeuristics("config/heuristics.cfg", std::ios::in);
    float val = 0;
    CString num;
    
    fHeuristics >> val;
    num.Format(_T("%1.3f"), val);
    m_EditDispersion.SetWindowText(num);
    m_SlideDisp.SetPos((long)(val * 1000));

    fHeuristics >> val;
    num.Format(_T("%d"), (long)val);
    m_EditIntersection.SetWindowText(num);
    m_SliderIntersect.SetPos((long)val);

    fHeuristics >> val;
    num.Format(_T("%d"), (long)val);
    m_EditPercToHtml.SetWindowText(num);
    m_SliderHTML.SetPos((long)val);

    fHeuristics >> val;
    num.Format(_T("%d"), (long)val);
    m_EditPercToVisibleHTML.SetWindowText(num);
    m_SliderVisibleHTML.SetPos((long)val);

    fHeuristics.close();

    // Выделяем 1ю строчку
    m_SiteSelect.SetCurSel(0);
    Rebuild();

    UpdateData(FALSE);
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
    news.Init();
    news.GetPossibleRanges();
    m_TagRanges = news.GetRanges();

    std::sort(m_TagRanges.begin(), m_TagRanges.end(), sortPredicate());

    m_RichCtrl.HideSelection(FALSE, FALSE);
    m_RichCtrl.SetDefaultCharFormat(cfDefault);
    m_RichCtrl.SetWindowText(m_FileData.c_str());
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
        BuildRanges();
    }
}

void CNewsColoringDlg::BuildRanges()
{
    m_DisplayedTagRanges.clear();
    for (vector<CTagSequence>::iterator it = m_TagRanges.begin(); it != m_TagRanges.end(); ++it)
    {
        if(it->percToVisibleHtml >= (double)m_SliderVisibleHTML.GetPos() &&
           it->percToHtml >= (double)m_SliderHTML.GetPos() &&
           it->dispersion <= (double)m_SlideDisp.GetPos() / 1000 &&
           it->innerIntersect <= m_SliderIntersect.GetPos())
        {
            m_DisplayedTagRanges.push_back(*it);
            CString str;
            str.AppendFormat(_T("%d - "), it->tagRange.size());
            for (vector<CTagRange>::iterator jt = it->tagRange.begin(); jt != it->tagRange.end(); ++jt)
            {
                str.AppendFormat(_T("(%d,%d) "), jt->begin, jt->end);
            }
            m_ListFreq.AddString(str.GetString());
        }
    }
}

void CNewsColoringDlg::ClearDialog()
{
    if(!UpdateData(TRUE))
        return;

    // очищаем listbox'ы
    m_ListFreq.ResetContent();
    m_ListRanges.ResetContent();
    m_PropFreq.ResetContent();
    m_PropRanges.ResetContent();

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
    int sz = (m_DisplayedTagRanges.begin() + selElem)->tagRange.size();

    for (vector<CTagRange>::iterator it = (m_DisplayedTagRanges.begin() + selElem)->tagRange.begin();
                                     it != (m_DisplayedTagRanges.begin() + selElem)->tagRange.end(); ++it)
    {
        CString str;
        str.AppendFormat(_T("(%d,%d) , len = %d, perc = %f, perc to vis = %f, intersection = %d"), it->begin, it->end, it->end - it->begin, it->percToHtml, it->percToVisibleHtml, it->innerIntersection);
        m_ListRanges.AddString(str.GetString());
    }

    m_PropFreq.ResetContent();
    CString format;
    format.AppendFormat(_T("Dispersion : %f"), (m_DisplayedTagRanges.begin() + selElem)->dispersion);
    m_PropFreq.AddString(format.GetString());
    format.Empty();
    format.AppendFormat(_T("Perc to html : %f"), (m_DisplayedTagRanges.begin() + selElem)->percToHtml);
    m_PropFreq.AddString(format.GetString());
    format.Empty();
    format.AppendFormat(_T("Perc to vis html : %f"), (m_DisplayedTagRanges.begin() + selElem)->percToVisibleHtml);
    m_PropFreq.AddString(format.GetString());
    format.Empty();
    format.AppendFormat(_T("Intersection : %d"), (m_DisplayedTagRanges.begin() + selElem)->innerIntersect);
    m_PropFreq.AddString(format.GetString());

    UpdateData(FALSE);
}

void CNewsColoringDlg::OnLbnSelchangeListranges()
{
    if(!UpdateData(TRUE))
        return;

    int selElem = m_ListFreq.GetCurSel();
    int selRange = m_ListRanges.GetCurSel();
    COLORREF color = RGB(255 * m_RadioRed, 255 * m_RadioGreen, 255 * m_RadioBlue);
    ColorRichText(((m_DisplayedTagRanges.begin() + selElem)->tagRange.begin() + selRange)->begin, ((m_DisplayedTagRanges.begin() + selElem)->tagRange.begin() + selRange)->end + 1, color);

    CTagRange seq = *((m_DisplayedTagRanges.begin() + selElem)->tagRange.begin() + selRange);
    m_PropRanges.ResetContent();
    CString format;
    format.AppendFormat(_T("Perc to html : %f"), seq.percToHtml);
    m_PropRanges.AddString(format.GetString());
    format.Empty();
    format.AppendFormat(_T("Perc to vis html : %f"), seq.percToVisibleHtml);
    m_PropRanges.AddString(format.GetString());
    format.Empty();
    format.AppendFormat(_T("Intersection : %d"), seq.innerIntersection);
    m_PropRanges.AddString(format.GetString());

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
    for(int i = 0; i < (m_DisplayedTagRanges.begin() + m_NewsBeginNum)->tagRange.size(); ++i)
    {
        string res = "";
        if (!m_IsSingleTagSeq)
        {
            res = string(m_FileData, ((m_DisplayedTagRanges.begin() + m_NewsBeginNum)->tagRange.begin() +  i)->begin,
                                     ((m_DisplayedTagRanges.begin() + m_NewsEndNum)->tagRange.begin() +  i)->end - ((m_DisplayedTagRanges.begin() + m_NewsBeginNum)->tagRange.begin() +  i)->begin + 1);
        }
        else
        {
            res = string(m_FileData, ((m_DisplayedTagRanges.begin() + m_NewsBeginNum)->tagRange.begin() +  i)->begin,
                                     ((m_DisplayedTagRanges.begin() + m_NewsBeginNum)->tagRange.begin() +  i)->end - ((m_DisplayedTagRanges.begin() + m_NewsBeginNum)->tagRange.begin() +  i)->begin + 1);
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
    int sz = (m_DisplayedTagRanges.begin() + selElem)->tagRange.size();
    COLORREF color = RGB(255 * m_RadioRed, 255 * m_RadioGreen, 255 * m_RadioBlue);

    for (vector<CTagRange>::iterator it = (m_DisplayedTagRanges.begin() + selElem)->tagRange.begin(); it != (m_DisplayedTagRanges.begin() + selElem)->tagRange.end(); ++it)
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
    for (vector<CTagSequence>::iterator it = m_DisplayedTagRanges.begin(); it < m_DisplayedTagRanges.end(); ++it)
    {
        ind++;
        for (vector<CTagRange>::iterator jt = it->tagRange.begin(); jt < it->tagRange.end(); ++jt)
        {
            if (jt->begin == begin && jt->end == end)
            {
                m_ListFreq.SetCurSel(ind - 1);
                it = m_DisplayedTagRanges.end() - 1;
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

void CNewsColoringDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    switch(nSBCode)
    {
        // release
        case SB_THUMBPOSITION:
        {
            break;
        }
           
        // scroll
        case SB_THUMBTRACK:
        {
            // get slider control
            CSliderCtrl *pSCtrl = (CSliderCtrl *) pScrollBar;
            CString position;

            if (pSCtrl == &m_SlideDisp)
            {
                float pos = (float)pSCtrl->GetPos() / 1000;
                position.Format(_T("%1.3f"), pos);
                m_EditDispersion.SetWindowText(position);
            }

            if (pSCtrl == &m_SliderIntersect)
            {
                position.Format(_T("%d"), pSCtrl->GetPos());
                m_EditIntersection.SetWindowText(position);
            }

            if (pSCtrl == &m_SliderHTML)
            {
                position.Format(_T("%d"), pSCtrl->GetPos());
                m_EditPercToHtml.SetWindowText(position);
            }

            if (pSCtrl == &m_SliderVisibleHTML)
            {
                position.Format(_T("%d"), pSCtrl->GetPos());
                m_EditPercToVisibleHTML.SetWindowText(position);
            }
            // очищаем listbox'ы
            m_ListFreq.ResetContent();
            m_ListRanges.ResetContent();
            m_PropFreq.ResetContent();
            m_PropRanges.ResetContent();
            BuildRanges();
            UpdateData(FALSE);
            break;
        }

        case SB_ENDSCROLL:
        {
            // get slider control
            CSliderCtrl *pSCtrl = (CSliderCtrl *) pScrollBar;
            CString position;
            if (pSCtrl == &m_SlideDisp)
            {
                float pos = (float)pSCtrl->GetPos() / 1000;
                position.Format(_T("%1.3f"), pos);
                m_EditDispersion.SetWindowText(position);
            }

            if (pSCtrl == &m_SliderIntersect)
            {
                position.Format(_T("%d"), pSCtrl->GetPos());
                m_EditIntersection.SetWindowText(position);
            }

            if (pSCtrl == &m_SliderHTML)
            {
                position.Format(_T("%d"), pSCtrl->GetPos());
                m_EditPercToHtml.SetWindowText(position);
            }

            if (pSCtrl == &m_SliderVisibleHTML)
            {
                position.Format(_T("%d"), pSCtrl->GetPos());
                m_EditPercToVisibleHTML.SetWindowText(position);
            }

            // очищаем listbox'ы
            m_ListFreq.ResetContent();
            m_ListRanges.ResetContent();
            m_PropFreq.ResetContent();
            m_PropRanges.ResetContent();
            BuildRanges();
            UpdateData(FALSE);
            break;
        }

        default:
            CDialog::OnHScroll(nSBCode, nPos, pScrollBar); 
    }
}
void CNewsColoringDlg::OnBnClickedBtnsaveheuristics()
{
    fstream fHeuristics("config/heuristics.cfg", std::ios::out);
    
    CString text;
    m_EditDispersion.GetWindowText(text);
    fHeuristics << text.GetString() << std::endl;
    m_EditIntersection.GetWindowText(text);
    fHeuristics << text.GetString() << std::endl;
    m_EditPercToHtml.GetWindowText(text);
    fHeuristics << text.GetString() << std::endl;
    m_EditPercToVisibleHTML.GetWindowText(text);
    fHeuristics << text.GetString() << std::endl;
    fHeuristics.close();
}

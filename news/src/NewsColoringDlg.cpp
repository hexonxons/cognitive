// NewsColoringDlg.cpp : ���� ����������
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


// ���������� ���� CNewsColoringDlg
CNewsColoringDlg::CNewsColoringDlg(CWnd* pParent /*=NULL*/)
: CDialog(CNewsColoringDlg::IDD, pParent)
, m_RadioBlue(1)
, m_RadioGreen(0)
, m_RadioRed(0)
, m_NewsBeginNum(-1)
, m_NewsEndNum(-1)
, m_IsSingleTagSeq(false)
, m_NewsNumber(0)
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
    DDX_Text(pDX, IDC_EDITNEWSNUMBER, m_NewsNumber);
    DDX_Control(pDX, IDC_FILESELECT, m_FileSelect);
    DDV_MinMaxLong(pDX, m_NewsNumber, 0, 1000);
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
    ON_EN_CHANGE(IDC_EDITNEWSNUMBER, &CNewsColoringDlg::OnEnChangeNewsnumber)
    ON_CBN_SELCHANGE(IDC_FILESELECT, &CNewsColoringDlg::OnCbnSelchangeFileselect)
    ON_EN_KILLFOCUS(IDC_EDITNEWSNUMBER, &CNewsColoringDlg::OnEnKillfocusEditnewsnumber)
END_MESSAGE_MAP()


// ����������� ��������� CNewsColoringDlg
BOOL CNewsColoringDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ������ ������ ��� ����� ����������� ����. ����� ������ ��� �������������,
    //  ���� ������� ���� ���������� �� �������� ����������
    SetIcon(m_hIcon, TRUE);			// ������� ������
    SetIcon(m_hIcon, FALSE);		// ������ ������
    
    m_SliderHTML.SetRange(0, 100, 1);
    m_SliderVisibleHTML.SetRange(0, 100, 1);
    m_SliderIntersect.SetRange(0, 150000, 1);
    m_SlideDisp.SetRange(0, 1000, 1);

    GetNewsFilenames();

    // ��������� ����� URL-�� ��������� ������
    fstream fSites("config/sites.cfg", std::ios::in);
    m_SiteSelect.AddString("");
    while (!fSites.eof())
    {
        string siteURI = "";
        fSites >> siteURI;
        m_SiteSelect.AddString(siteURI.c_str());
    }
    fSites.close();

    // ��������� ��������� ���������
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

    // �������� 1� �������
    m_SiteSelect.SetCurSel(0);
    RebuildURL();

    UpdateData(FALSE);
    return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������. ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.

void CNewsColoringDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // �������� ���������� ��� ���������

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // ������������ ������ �� ������ ����������� ��������������
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // ��������� ������
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
HCURSOR CNewsColoringDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CNewsColoringDlg::Init(string fileData, int minWordlen, int minFreq, int newsNum)
{
    struct sortPredicate
    {
        bool operator () (const CNewsBlock &left, const CNewsBlock &right)
        {
            return left.NewsRange.size() > right.NewsRange.size();
        }
    };

    CNewsFinder news(fileData.c_str(), minWordlen, minFreq);
    m_FileData = fileData;
    news.Init();
    news.GetPossibleRanges();
    m_TagRanges = news.GetRanges();
    m_AvgDispersion = news.getAvgDispersion();
    //std::sort(m_TagRanges.begin(), m_TagRanges.end(), sortPredicate());

    m_RichCtrl.HideSelection(FALSE, FALSE);
    m_RichCtrl.SetDefaultCharFormat(cfDefault);
    m_RichCtrl.SetWindowText(m_FileData.c_str());
}

void CNewsColoringDlg::RebuildURL()
{
    // ������� ���� ������
    ClearDialog();
    if(strlen(m_SiteURL.GetString()) != 0)
    {
        // �������� html-��� ��������
        LPSTR URLContent = GetPageSource(m_SiteURL.GetString());
        Init(URLContent, 4, 8, 0);
        BuildRanges();
    }
}

void CNewsColoringDlg::RebuildFILE()
{
    // ������� ���� ������
    ClearDialog();
    if(strlen(m_SiteURL.GetString()) != 0)
    {
        // �������� html-��� ��������
        string filename = string("pages/") + string(m_SiteURL.GetString());
        std::fstream fileIn(filename.c_str(), std::ios::in);
        m_FileData = std::string((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
        fileIn.close();
        Init(m_FileData, 4, 8, 0);
        BuildRanges();
    }
}

void CNewsColoringDlg::BuildRanges()
{
    // ������� listbox'�
    m_ListFreq.ResetContent();
    m_ListRanges.ResetContent();
    m_PropFreq.ResetContent();
    m_PropRanges.ResetContent();

    m_DisplayedTagRanges.clear();

    for (vector<CNewsBlock>::iterator it = m_TagRanges.begin(); it != m_TagRanges.end(); ++it)
    {
        NewsBlockHeuristics heusristics = it->BlockHeuristics;
        if(heusristics.PercentageToAllVisibleHtmlLen >= (double)m_SliderVisibleHTML.GetPos() &&
            heusristics.PercentageToAllHtmlLen >= (double)m_SliderHTML.GetPos() && 
            heusristics.Dispersion < m_AvgDispersion)
        {
            if (m_NewsNumber != 0)
            {
                if (it->NewsRange.size() != m_NewsNumber)
                    continue;
            }
            m_DisplayedTagRanges.push_back(*it); 
        }
    }

    for (vector<CNewsBlock>::iterator it = m_DisplayedTagRanges.begin(); it != m_DisplayedTagRanges.end(); ++it)
    {
        CString str;
        str.AppendFormat(_T("%d - "), it->NewsRange.size());
        for (vector<CNews>::iterator jt = it->NewsRange.begin(); jt != it->NewsRange.end(); ++jt)
        {
            str.AppendFormat(_T("(%d,%d) "), jt->NewsBegin, jt->NewsEnd);
        }
        m_ListFreq.AddString(str.GetString());
    }
}

std::vector<CNews> CNewsColoringDlg::FinishTagsSeq(std::vector<CNews> &seq)
{
    return std::vector<CNews>();
}

void CNewsColoringDlg::ClearDialog()
{
    if(!UpdateData(TRUE))
        return;

    // ������� listbox'�
    m_ListFreq.ResetContent();
    m_ListRanges.ResetContent();
    m_PropFreq.ResetContent();
    m_PropRanges.ResetContent();

    // ������ �����
    m_RadioBlue = false;
    m_RadioGreen = false;
    m_RadioRed = true;

    // ������� �����
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
    int sz = (m_DisplayedTagRanges.begin() + selElem)->NewsRange.size();

    for (vector<CNews>::iterator it = (m_DisplayedTagRanges.begin() + selElem)->NewsRange.begin();
                                     it != (m_DisplayedTagRanges.begin() + selElem)->NewsRange.end(); ++it)
    {
        CString str;
        str.AppendFormat(_T("(%d,%d) , len = %d, perc = %f, perc to vis = %f"), it->NewsBegin, it->NewsEnd, it->NewsEnd - it->NewsBegin, it->Heuristics.PercentageToAllHtmlLen, it->Heuristics.PercentageToAllVisibleHtmlLen);
        m_ListRanges.AddString(str.GetString());
    }

    m_PropFreq.ResetContent();
    CString format;
    format.AppendFormat(_T("Dispersion : %f"), (m_DisplayedTagRanges.begin() + selElem)->BlockHeuristics.Dispersion);
    m_PropFreq.AddString(format.GetString());
    format.Empty();
    format.AppendFormat(_T("Intersection : %d"), (m_DisplayedTagRanges.begin() + selElem)->BlockHeuristics.IntersectionWithOtherBlocks);
    m_PropFreq.AddString(format.GetString());
    format.Empty();
    format.AppendFormat(_T("Perc to html : %f"), (m_DisplayedTagRanges.begin() + selElem)->BlockHeuristics.PercentageToAllHtmlLen);
    m_PropFreq.AddString(format.GetString());
    format.Empty();
    format.AppendFormat(_T("Perc to vis html : %f"), (m_DisplayedTagRanges.begin() + selElem)->BlockHeuristics.PercentageToAllVisibleHtmlLen);
    m_PropFreq.AddString(format.GetString());
    format.Empty();
    format.AppendFormat(_T("Weight : %d"), (m_DisplayedTagRanges.begin() + selElem)->BlockHeuristics.Weight);
    m_PropFreq.AddString(format.GetString());
    format.Empty();
    format.Empty();

    UpdateData(FALSE);
}

void CNewsColoringDlg::OnLbnSelchangeListranges()
{
    if(!UpdateData(TRUE))
        return;

    int selElem = m_ListFreq.GetCurSel();
    int selRange = m_ListRanges.GetCurSel();
    COLORREF color = RGB(255 * m_RadioRed, 255 * m_RadioGreen, 255 * m_RadioBlue);
    NewsHeuristics heuristics = ((m_DisplayedTagRanges.begin() + selElem)->NewsRange.begin() + selRange)->Heuristics;

    ColorRichText(((m_DisplayedTagRanges.begin() + selElem)->NewsRange.begin() + selRange)->NewsBegin, ((m_DisplayedTagRanges.begin() + selElem)->NewsRange.begin() + selRange)->NewsEnd + 1, color);

    m_PropRanges.ResetContent();
    CString format;
    format.AppendFormat(_T("Perc to html : %f"), heuristics.PercentageToAllHtmlLen);
    m_PropRanges.AddString(format.GetString());
    format.Empty();
    format.AppendFormat(_T("Perc to vis html : %f"), heuristics.PercentageToAllVisibleHtmlLen);
    m_PropRanges.AddString(format.GetString());
    format.Empty();
    format.AppendFormat(_T("Distance to next news : %d"), heuristics.DistanceToNextNews);
    m_PropRanges.AddString(format.GetString());

    UpdateData(FALSE);
}

// ������ ��������� � ������
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
    if(!UpdateData(TRUE))
        return;

    ///< �������� ����.
    std::fstream fileOut("out.html", std::ios::out);

    fileOut << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>\n<body>\n";

    unsigned int offset = 0;
    // �������� ������� ������� � ������� �
    for(int i = 0; i < (m_DisplayedTagRanges.begin() + m_NewsBeginNum)->NewsRange.size(); ++i)
    {
        string res = "";
        if (!m_IsSingleTagSeq)
        {
            res = string(m_FileData, ((m_DisplayedTagRanges.begin() + m_NewsBeginNum)->NewsRange.begin() +  i)->NewsBegin,
                                     ((m_DisplayedTagRanges.begin() + m_NewsEndNum)->NewsRange.begin() +  i)->NewsEnd - ((m_DisplayedTagRanges.begin() + m_NewsBeginNum)->NewsRange.begin() +  i)->NewsBegin + 1);
        }
        else
        {
            res = string(m_FileData, ((m_DisplayedTagRanges.begin() + m_NewsBeginNum)->NewsRange.begin() +  i)->NewsBegin,
                                     ((m_DisplayedTagRanges.begin() + m_NewsBeginNum)->NewsRange.begin() +  i)->NewsEnd - ((m_DisplayedTagRanges.begin() + m_NewsBeginNum)->NewsRange.begin() +  i)->NewsBegin + 1);
        }
        fileOut << res;
        fileOut << "\n<br>#########################################################################################\n";
    }
    fileOut << "\n</body></html>";
    fileOut.close();
    UpdateData(FALSE);
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
    int sz = (m_DisplayedTagRanges.begin() + selElem)->NewsRange.size();
    COLORREF color = RGB(255 * m_RadioRed, 255 * m_RadioGreen, 255 * m_RadioBlue);

    for (vector<CNews>::iterator it = (m_DisplayedTagRanges.begin() + selElem)->NewsRange.begin(); it != (m_DisplayedTagRanges.begin() + selElem)->NewsRange.end(); ++it)
    {
        ColorRichText(it->NewsBegin, it->NewsEnd + 1, color);
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
    for (vector<CNewsBlock>::iterator it = m_DisplayedTagRanges.begin(); it < m_DisplayedTagRanges.end(); ++it)
    {
        ind++;
        for (vector<CNews>::iterator jt = it->NewsRange.begin(); jt < it->NewsRange.end(); ++jt)
        {
            if (jt->NewsBegin == begin && jt->NewsEnd == end)
            {
                m_ListFreq.SetCurSel(ind - 1);
                it = m_DisplayedTagRanges.end() - 1;
                break;
            }

            if (jt->NewsBegin > begin)
            {
                break;
            }
        }
    }
    m_ListFreq.SetSel(-1, false);
    UpdateData(FALSE);
}

// ��������� URL �� ����������� ������
void CNewsColoringDlg::OnCbnSelchangeSiteselect()
{
    m_SiteURL.ReleaseBuffer();
    int num = m_SiteSelect.GetCurSel();
    int sz = m_SiteSelect.GetLBTextLen(num);
    m_SiteSelect.GetLBText(num, m_SiteURL.GetBuffer(sz));
    RebuildURL();
}

// ����������� ������ ����� ��������� ������
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
            // ������� listbox'�
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

            // ������� listbox'�
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

void CNewsColoringDlg::OnEnChangeNewsnumber()
{
    if(!UpdateData(TRUE))
        return;

    
    //m_NewsNumber++;
    UpdateData(FALSE);
}

void CNewsColoringDlg::OnEnKillfocusEditnewsnumber()
{
    if(!UpdateData(TRUE))
        return;

    BuildRanges();

    UpdateData(FALSE);
}

void CNewsColoringDlg::GetNewsFilenames()
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    hFind = FindFirstFile("pages/*", &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE) 
    {
        printf ("FindFirstFile failed (%d)\n", GetLastError());
        return;
    } 
    FindNextFile(hFind, &FindFileData);
    FindNextFile(hFind, &FindFileData);
    do
    {
        m_FileSelect.AddString(FindFileData.cFileName);
    }
    while (FindNextFile(hFind, &FindFileData) != 0);
}


void CNewsColoringDlg::OnCbnSelchangeFileselect()
{
    m_SiteURL.ReleaseBuffer();
    int num = m_FileSelect.GetCurSel();
    int sz = m_FileSelect.GetLBTextLen(num);
    m_FileSelect.GetLBText(num, m_SiteURL.GetBuffer(sz));
    RebuildFILE();
}

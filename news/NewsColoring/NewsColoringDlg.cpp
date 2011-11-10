
// NewsColoringDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "NewsColoring.h"
#include "NewsColoringDlg.h"
#include "tagprocess.h"
#include <algorithm>
#include <fstream>


using std::string;
using std::vector;
using std::pair;
using std::make_pair;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// диалоговое окно CNewsColoringDlg




CNewsColoringDlg::CNewsColoringDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewsColoringDlg::IDD, pParent)
    , m_fileName(_T("news"))
    , m_minFreq(8)
    , m_minWordlen(8)
    , m_newsNum(0)
    , m_radioBlue(1)
    , m_radioGreen(0)
    , m_radioRed(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    AfxInitRichEdit2();

    cfDefault.dwMask = CFM_CHARSET | CFM_FACE | CFM_SIZE | CFM_OFFSET | CFM_COLOR;
    cfDefault.dwMask ^= CFM_ITALIC ^ CFM_BOLD ^ CFM_STRIKEOUT ^ CFM_UNDERLINE;;
    cfDefault.dwEffects = 0;
    cfDefault.yHeight = 180; //10pts * 20 twips/point = 200 twips
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

    DDX_Text(pDX, IDC_EDITFILENAME, m_fileName);
    DDV_MaxChars(pDX, m_fileName, 20);

    DDX_Text(pDX, IDC_EDITFREQ, m_minFreq);
    DDV_MinMaxInt(pDX, m_minFreq, 1, 100);

    DDX_Text(pDX, IDC_EDITLEN, m_minWordlen);
    DDV_MinMaxInt(pDX, m_minWordlen, 1, 100);

    DDX_Text(pDX, IDC_EDITNUM, m_newsNum);
    DDV_MinMaxInt(pDX, m_newsNum, 0, 100);
    DDX_Control(pDX, IDC_LIST, m_ListBox);
    DDX_Control(pDX, IDC_LISTRANGES, m_ListRanges);
}

BEGIN_MESSAGE_MAP(CNewsColoringDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTNRUN, &CNewsColoringDlg::OnBnClickedBtnrun)
    ON_BN_CLICKED(IDC_RADIORED, &CNewsColoringDlg::OnBnClickedRadiored)
    ON_BN_CLICKED(IDC_RADIOGREEN, &CNewsColoringDlg::OnBnClickedRadiogreen)
    ON_BN_CLICKED(IDC_RADIOBLUE, &CNewsColoringDlg::OnBnClickedRadioblue)
    ON_LBN_SELCHANGE(IDC_LIST, &CNewsColoringDlg::OnLbnSelchangeList)
    ON_LBN_SELCHANGE(IDC_LISTRANGES, &CNewsColoringDlg::OnLbnSelchangeListranges)
    ON_BN_CLICKED(IDC_BTNRESETSEL, &CNewsColoringDlg::OnBnClickedBtnresetsel)
END_MESSAGE_MAP()


// обработчики сообщений CNewsColoringDlg

BOOL CNewsColoringDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

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

struct pred
{
    bool operator () (const vector<pair<int, int>> &left, const vector<pair<int, int>> &right)
    {
        return left.size() > right.size();
    }
};

void CNewsColoringDlg::ColorRichText(int start, int end, COLORREF color)
{
    m_RichCtrl.SetRedraw(FALSE);
    CHARFORMAT2 cfNew = cfDefault;
    cfNew.crTextColor = color;
    
    m_RichCtrl.SetSel(start, end);
    m_RichCtrl.SetSelectionCharFormat(cfNew);
    m_RichCtrl.SetSel(0,0);

    m_RichCtrl.SetRedraw(TRUE);
    m_RichCtrl.RedrawWindow();
}

void CNewsColoringDlg::OnBnClickedBtnrun()
{
    if(!UpdateData(TRUE))
        return;

    CNewsFinder news(m_fileName.GetString(), m_minWordlen, m_minFreq, m_newsNum);

    std::fstream fileIn(m_fileName.GetString(), std::ios::in);
    m_fileData = std::string((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    fileIn.close();

    vector<pair<string, string>> remDoubleTag;
    vector<string> remTag;
    remDoubleTag.push_back(make_pair("<script>", "</script>"));
    remDoubleTag.push_back(make_pair("<noscript>", "</noscript>"));
    remDoubleTag.push_back(make_pair("<form>", "</form>"));
    remDoubleTag.push_back(make_pair("<iframe>", "</iframe>"));
    remDoubleTag.push_back(make_pair("<noindex> ", "</noindex> "));
    remDoubleTag.push_back(make_pair("<style>", "</style>"));

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
    vector<CTagSequence> seq = news.GetRanges();

    for (vector<CTagSequence>::iterator it = seq.begin(); it != seq.end(); ++it)
    {
        tagRanges.push_back(it->tagRange);
    }
    std::sort(tagRanges.begin(), tagRanges.end(),pred());

    m_RichCtrl.SetDefaultCharFormat(cfDefault);
    m_RichCtrl.SetWindowText(m_fileData.c_str());
    
    for (vector<vector<pair<int, int>>>::iterator it = tagRanges.begin(); it != tagRanges.end(); ++it)
    {
        CString str;
        str.AppendFormat(_T("%d - "), it->size());
        for (vector<pair<int, int>>::iterator jt = it->begin(); jt != it->end(); ++jt)
        {
            str.AppendFormat(_T("(%d,%d) "), jt->first, jt->second);
        }
        m_ListBox.AddString(str.GetString());
    }
    UpdateData(FALSE);
}

void CNewsColoringDlg::OnBnClickedRadiored()
{
    m_radioBlue = false;
    m_radioGreen = false;
    m_radioRed = true;
}

void CNewsColoringDlg::OnBnClickedRadiogreen()
{
    m_radioBlue = false;
    m_radioRed = false;
    m_radioGreen = true;
}

void CNewsColoringDlg::OnBnClickedRadioblue()
{
     m_radioBlue = true;
     m_radioRed = false;
     m_radioGreen = false;
}

void CNewsColoringDlg::OnLbnSelchangeList()
{
    if(!UpdateData(TRUE))
        return;

    m_ListRanges.ResetContent();
    // Get the indexes of all the selected items.
    int selElem = m_ListBox.GetCurSel();
    int sz = (tagRanges.begin() + selElem)->size();

    for (vector<pair<int, int>>::iterator it = (tagRanges.begin() + selElem)->begin(); it != (tagRanges.begin() + selElem)->end(); ++it)
    {
        CString str;
        str.AppendFormat(_T("(%d,%d) "), it->first, it->second);
        m_ListRanges.AddString(str.GetString());
    }
    
    UpdateData(FALSE);
}

void CNewsColoringDlg::OnLbnSelchangeListranges()
{
    if(!UpdateData(TRUE))
        return;

    int selElem = m_ListBox.GetCurSel();
    int selRange = m_ListRanges.GetCurSel();
    COLORREF color = RGB(0, 0, 255);

    if (m_radioRed)
    {
        color = RGB(255, 0, 0);
    }

    if (m_radioGreen)
    {
        color = RGB(0, 255, 0);
    }

    if (m_radioBlue)
    {
        color = RGB(0, 0, 255);
    }

    ColorRichText(((tagRanges.begin() + selElem)->begin() + selRange)->first, ((tagRanges.begin() + selElem)->begin() + selRange)->second + 1, color);

    UpdateData(FALSE);
}

void CNewsColoringDlg::OnBnClickedBtnresetsel()
{    
    if(!UpdateData(TRUE))
        return;
    cfDefault.crTextColor = RGB(0,0,0);
    m_RichCtrl.SetDefaultCharFormat(cfDefault);
    m_RichCtrl.SetWindowText(m_fileData.c_str());
    UpdateData(FALSE);
}

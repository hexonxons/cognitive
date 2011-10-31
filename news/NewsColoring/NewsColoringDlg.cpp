
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
    , m_FileName(_T(""))
    , m_minFreq(0)
    , m_minWordlen(0)
    , m_newsNum(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    AfxInitRichEdit2();
}

void CNewsColoringDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RICHEDIT, m_RichCtrl);

    DDX_Text(pDX, IDC_EDITFILENAME, m_FileName);
    DDV_MaxChars(pDX, m_FileName, 20);

    DDX_Text(pDX, IDC_EDITFREQ, m_minFreq);
    DDV_MinMaxInt(pDX, m_minFreq, 1, 100);

    DDX_Text(pDX, IDC_EDITLEN, m_minWordlen);
    DDV_MinMaxInt(pDX, m_minWordlen, 1, 100);

    DDX_Text(pDX, IDC_EDITNUM, m_newsNum);
	DDV_MinMaxInt(pDX, m_newsNum, 0, 100);
}

BEGIN_MESSAGE_MAP(CNewsColoringDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTNRUN, &CNewsColoringDlg::OnBnClickedBtnrun)
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

void ColorRichText(CRichEditCtrl &ctrlText, int start, int end, COLORREF color)
{
    ctrlText.SetRedraw(FALSE);
    CHARFORMAT2 cfDefault;
    cfDefault.dwMask = CFM_CHARSET | CFM_FACE | CFM_SIZE | CFM_OFFSET | CFM_COLOR;
    cfDefault.dwMask ^= CFM_ITALIC ^ CFM_BOLD ^ CFM_STRIKEOUT ^ CFM_UNDERLINE;;
    cfDefault.dwEffects = 0;
    cfDefault.yHeight = 180; //10pts * 20 twips/point = 200 twips
    cfDefault.bCharSet = OEM_CHARSET;
    cfDefault.bPitchAndFamily = FIXED_PITCH | FF_MODERN;
    cfDefault.yOffset = 0;
    cfDefault.crTextColor = color;
    strcpy(cfDefault.szFaceName, "Courier New");
    cfDefault.cbSize = sizeof(cfDefault);
    
    ctrlText.SetSel(start, end);
    ctrlText.SetSelectionCharFormat(cfDefault);
    ctrlText.SetSel(0,0);

    ctrlText.SetRedraw(TRUE);
    ctrlText.RedrawWindow();
}

void CNewsColoringDlg::OnBnClickedBtnrun()
{
    if(!UpdateData(TRUE))
        return;

    CNewsFinder news(m_FileName.GetString(), m_minWordlen, m_minFreq, m_newsNum);

    std::fstream fileIn(m_FileName.GetString(), std::ios::in);
    string m_fileData = std::string((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
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
    vector<vector<pair<int, int>>> tagRanges;

    for (vector<CTagSequence>::iterator it = seq.begin(); it != seq.end(); ++it)
    {
        tagRanges.push_back(it->tagRange);
    }
    std::sort(tagRanges.begin(), tagRanges.end(),pred());

    CHARFORMAT2 cfDefault;
    cfDefault.dwMask = CFM_CHARSET | CFM_FACE | CFM_SIZE | CFM_OFFSET | CFM_COLOR;
    cfDefault.dwMask ^= CFM_ITALIC ^ CFM_BOLD ^ CFM_STRIKEOUT ^ CFM_UNDERLINE;;
    cfDefault.dwEffects = 0;
    cfDefault.yHeight = 180; //10pts * 20 twips/point = 200 twips
    cfDefault.bCharSet = OEM_CHARSET;
    cfDefault.bPitchAndFamily = FIXED_PITCH | FF_MODERN;
    cfDefault.yOffset = 0;
    cfDefault.crTextColor = RGB(0,0,0);
    strcpy(cfDefault.szFaceName, "Courier New");
    cfDefault.cbSize = sizeof(cfDefault);
    m_RichCtrl.SetDefaultCharFormat(cfDefault);

    m_RichCtrl.SetWindowText(m_fileData.c_str());


    for (vector<vector<pair<int, int>>>::iterator it = tagRanges.begin(); it != tagRanges.end(); ++it)
    {
        int sz = it[0].size();
        COLORREF color;
        if (sz == m_newsNum)
        {
            color = RGB(255, 0, 0);
        }
        else
            if (sz > m_newsNum)
            {
                color = RGB(0, 255, 0);
            }
            else
                continue;

        for (vector<pair<int, int>>::iterator jt = it[0].begin(); jt != it[0].end(); ++jt)
        {
            ColorRichText(m_RichCtrl, jt->first, jt->second, color);
        }
    }

    UpdateData(FALSE);
}


// NewsColoringDlg.cpp : файл реализации
//

#include "stdafx.h"

#include <algorithm>
#include <fstream>
//#include "tagprocess.h"

#include "NewsColoring.h"
#include "NewsColoringDlg.h"
#include "InitDialog.h"

//using std::string;
using std::vector;
using std::pair;
//using std::make_pair;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// диалоговое окно CNewsColoringDlg




CNewsColoringDlg::CNewsColoringDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewsColoringDlg::IDD, pParent)
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

    DDX_Control(pDX, IDC_LIST, m_ListBox);
    DDX_Control(pDX, IDC_LISTRANGES, m_ListRanges);
    DDX_Control(pDX, IDC_DECODEDLIST, m_DecodedList);
}

BEGIN_MESSAGE_MAP(CNewsColoringDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_RADIORED, &CNewsColoringDlg::OnBnClickedRadiored)
    ON_BN_CLICKED(IDC_RADIOGREEN, &CNewsColoringDlg::OnBnClickedRadiogreen)
    ON_BN_CLICKED(IDC_RADIOBLUE, &CNewsColoringDlg::OnBnClickedRadioblue)
    ON_LBN_SELCHANGE(IDC_LIST, &CNewsColoringDlg::OnLbnSelchangeList)
    ON_LBN_SELCHANGE(IDC_LISTRANGES, &CNewsColoringDlg::OnLbnSelchangeListranges)
    ON_BN_CLICKED(IDC_BTNRESETSEL, &CNewsColoringDlg::OnBnClickedBtnresetsel)
    ON_BN_CLICKED(IDC_GETDECODED, &CNewsColoringDlg::OnBnClickedGetdecoded)
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
    //CInitDialog *InitDlg = new CInitDialog;
    //InitDlg->Create(IDD_INITDIALOG, this);
    CInitDialog InitDlg;
    if(InitDlg.DoModal())
    {
        UpdateData(TRUE);
        tagRanges = InitDlg.GetTagRanges();
        m_fileData = InitDlg.GetFileData();
        UpdateData(TRUE);
    }

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

    //InitDlg->ShowWindow(SW_SHOW);
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

void CNewsColoringDlg::OnBnClickedGetdecoded()
{
    // TODO: Add your control notification handler code here
}

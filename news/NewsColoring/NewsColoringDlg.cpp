
// NewsColoringDlg.cpp : файл реализации
//

#include "stdafx.h"

#include <algorithm>
#include <fstream>
#include <string>

#include "NewsColoring.h"
#include "NewsColoringDlg.h"
#include "InitDialog.h"

using std::string;
using std::vector;
using std::pair;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// диалоговое окно CNewsColoringDlg




CNewsColoringDlg::CNewsColoringDlg(CWnd* pParent /*=NULL*/)
: CDialog(CNewsColoringDlg::IDD, pParent)
, m_radioBlue(1)
, m_radioGreen(0)
, m_radioRed(0)
, newsBeginNum(-1)
, newsEndNum(-1)
, isSingleTagSeq(false)
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
    DDX_Control(pDX, IDC_DECODEDLIST, m_EncodedListLeft);
    DDX_Control(pDX, IDC_ENCODEDLISTRIGHT, m_EncodedListRight);
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
    ON_BN_CLICKED(IDC_GETDECODEDLEFT, &CNewsColoringDlg::OnBnClickedGetdecodedLeft)
    ON_BN_CLICKED(IDC_GETDECODEDRIGHT, &CNewsColoringDlg::OnBnClickedGetdecodedright)
    ON_BN_CLICKED(IDC_BTNSETBEGIN, &CNewsColoringDlg::OnBnClickedBtnsetbegin)
    ON_BN_CLICKED(IDC_BTNSETEND, &CNewsColoringDlg::OnBnClickedBtnsetend)
    ON_BN_CLICKED(IDC_BTNPRINTNEWS, &CNewsColoringDlg::OnBnClickedBtnprintnews)
    ON_BN_CLICKED(IDC_BNTSINGLE, &CNewsColoringDlg::OnBnClickedBntsingle)
    ON_BN_CLICKED(IDC_BTNCOLORALL, &CNewsColoringDlg::OnBnClickedBtncolorall)
END_MESSAGE_MAP()


// обработчики сообщений CNewsColoringDlg

BOOL CNewsColoringDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Задает значок для этого диалогового окна. Среда делает это автоматически,
    //  если главное окно приложения не является диалоговым
    SetIcon(m_hIcon, TRUE);			// Крупный значок
    SetIcon(m_hIcon, FALSE);		// Мелкий значок

    CInitDialog InitDlg;
    if(InitDlg.DoModal())
    {
        UpdateData(TRUE);
        tagRanges = InitDlg.GetTagRanges();
        m_fileData = InitDlg.GetFileData();
        UpdateData(TRUE);
    }

    m_RichCtrl.HideSelection(FALSE, FALSE);
    m_RichCtrl.SetDefaultCharFormat(cfDefault);
    m_RichCtrl.SetWindowText(m_fileData.c_str());

    OnBnClickedRadiored();

    for (vector<CTagSequence>::iterator it = tagRanges.begin(); it != tagRanges.end(); ++it)
    {
        CString str;
        str.AppendFormat(_T("%d - "), it->tagRange.size());
        for (vector<CTagRange>::iterator jt = it->tagRange.begin(); jt != it->tagRange.end(); ++jt)
        {
            str.AppendFormat(_T("(%d,%d) "), jt->begin, jt->end);
        }
        m_ListBox.AddString(str.GetString());
    }

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
    m_RichCtrl.SetSel(start, start);
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
    int sz = (tagRanges.begin() + selElem)->tagRange.size();

    for (vector<CTagRange>::iterator it = (tagRanges.begin() + selElem)->tagRange.begin(); it != (tagRanges.begin() + selElem)->tagRange.end(); ++it)
    {
        CString str;
        str.AppendFormat(_T("(%d,%d) , len = %d"), it->begin, it->end, it->end - it->begin);
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

    ColorRichText(((tagRanges.begin() + selElem)->tagRange.begin() + selRange)->begin, ((tagRanges.begin() + selElem)->tagRange.begin() + selRange)->end + 1, color);

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

void CNewsColoringDlg::EncodeString()
{
    if(!UpdateData(TRUE))
        return;

    string selection = (m_RichCtrl.GetSelText()).GetString();
    string tag;
    CTagDescription tagCode;
    encodedString.clear();

    for (int i = 0; i < selection.size(); ++i)
    {
        // если (возможно) открывающий тег
        if (selection[i] == '<')
        {
            // к примеру, знак < в js
            if (tagCode.nTagBegin != -1)
                ++i;
            else
                tagCode.nTagBegin = i;

            // пока не дойдем до пробела/закрывающей скобки - записываем
            while (selection[i] != ' ' &&selection[i] != '>')
            {
                tag += selection[i];
                tagCode.nTagCode += selection[i];
                ++i;				
            }
        }

        // если закрывающая скобка - записываем
        if (selection[i] == '>' && tagCode.nTagBegin != -1)
        {
            tag += selection[i];
            tagCode.nTagCode +=selection[i];
            tagCode.nTagEnd = i;

            if(tag[1] == '/')
                tagCode.bIsClose = 1;
            else
                tagCode.bIsClose = 0;

            tagCode.tag = tag;

           

            encodedString.push_back(tagCode);
            tagCode.Clear();
            tag = "";
        }
    }
}
void CNewsColoringDlg::OnBnClickedGetdecodedLeft()
{
    if(!UpdateData(TRUE))
        return;

    m_EncodedListLeft.ResetContent();
    EncodeString();
    for (int i = 0; i < encodedString.size(); ++i )
    {
        CString outString;
        CTagDescription tagCode = encodedString[i];
        outString.AppendFormat(_T("(| %s | %d | %d | %d | %d |)"), tagCode.tag.c_str(), tagCode.bIsClose, tagCode.nTagCode, tagCode.nTagBegin, tagCode.nTagEnd);
        m_EncodedListLeft.AddString(outString.GetString());
    }
   
    UpdateData(FALSE);
}

void CNewsColoringDlg::OnBnClickedGetdecodedright()
{
    if(!UpdateData(TRUE))
        return;

    m_EncodedListRight.ResetContent();
    EncodeString();
    for (int i = 0; i < encodedString.size(); ++i )
    {
        CString outString;
        CTagDescription tagCode = encodedString[i];
        outString.AppendFormat(_T("(| %s | %d | %d | %d | %d |)"), tagCode.tag.c_str(), tagCode.bIsClose, tagCode.nTagCode, tagCode.nTagBegin, tagCode.nTagEnd);
        m_EncodedListRight.AddString(outString.GetString());
    }

    UpdateData(FALSE);
}

void CNewsColoringDlg::OnBnClickedBtnsetbegin()
{
    if(!UpdateData(TRUE))
        return;

    newsBeginNum = m_ListBox.GetCurSel();

    UpdateData(FALSE);
}

void CNewsColoringDlg::OnBnClickedBtnsetend()
{
    if(!UpdateData(TRUE))
        return;

    newsEndNum = m_ListBox.GetCurSel();

    UpdateData(FALSE);
}

void CNewsColoringDlg::OnBnClickedBtnprintnews()
{
    ///< Выходной файл.
    std::fstream fileOut("out.html", std::ios::out);

    fileOut << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>\n<body>\n";

    unsigned int offset = 0;
    // Получаем строчку новости и выводим её
    for(int i = 0; i < (tagRanges.begin() + newsBeginNum)->tagRange.size(); ++i)
    {

        string res = "";
        if (!isSingleTagSeq)
        {
            res = string(m_fileData, ((tagRanges.begin() + newsBeginNum)->tagRange.begin() +  i)->begin,
                                     ((tagRanges.begin() + newsEndNum)->tagRange.begin() +  i)->end - ((tagRanges.begin() + newsBeginNum)->tagRange.begin() +  i)->begin + 1);
        }
        else
        {
            res = string(m_fileData, ((tagRanges.begin() + newsBeginNum)->tagRange.begin() +  i)->begin,
                                     ((tagRanges.begin() + newsBeginNum)->tagRange.begin() +  i)->end - ((tagRanges.begin() + newsBeginNum)->tagRange.begin() +  i)->begin + 1);
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

    isSingleTagSeq = true;
    newsBeginNum = m_ListBox.GetCurSel();

    UpdateData(FALSE);
}

void CNewsColoringDlg::OnBnClickedBtncolorall()
{
    if(!UpdateData(TRUE))
        return;

    int selElem = m_ListBox.GetCurSel();
    int sz = (tagRanges.begin() + selElem)->tagRange.size();
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

    for (vector<CTagRange>::iterator it = (tagRanges.begin() + selElem)->tagRange.begin(); it != (tagRanges.begin() + selElem)->tagRange.end(); ++it)
    {
        ColorRichText(it->begin, it->end + 1, color);
    }

    UpdateData(FALSE);
}

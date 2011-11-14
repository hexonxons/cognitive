// InitDialog.cpp : implementation file
//

#include "stdafx.h"

#include <algorithm>
#include <fstream>
#include "../newsfinder/tagprocess.h"

#include "NewsColoring.h"
#include "InitDialog.h"

using std::string;
using std::vector;
using std::pair;
using std::make_pair;

// CInitDialog dialog

IMPLEMENT_DYNAMIC(CInitDialog, CDialog)

CInitDialog::CInitDialog(CWnd* pParent /*=NULL*/)
: CDialog(CInitDialog::IDD, pParent) 
    , m_fileName(_T("news"))
    , m_minFreq(8)
    , m_minWordlen(8)
    , m_newsNum(0)
{
    m_pParentDialog = pParent;
}

CInitDialog::~CInitDialog()
{
}

void CInitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_EDITFILENAME, m_fileName);
    DDV_MaxChars(pDX, m_fileName, 20);

    DDX_Text(pDX, IDC_EDITFREQ, m_minFreq);
    DDV_MinMaxInt(pDX, m_minFreq, 1, 100);

    DDX_Text(pDX, IDC_EDITLEN, m_minWordlen);
    DDV_MinMaxInt(pDX, m_minWordlen, 1, 100);

    DDX_Text(pDX, IDC_EDITNUM, m_newsNum);
    DDV_MinMaxInt(pDX, m_newsNum, 0, 100);
}


BEGIN_MESSAGE_MAP(CInitDialog, CDialog)
    ON_BN_CLICKED(IDC_BTNRUN, &CInitDialog::OnBnClickedBtnrun)
END_MESSAGE_MAP()

struct pred
{
    bool operator () (const vector<pair<int, int>> &left, const vector<pair<int, int>> &right)
    {
        return left.size() > right.size();
    }
};


// CInitDialog message handlers
void CInitDialog::OnBnClickedBtnrun()
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
    


    //m_RichCtrl.SetDefaultCharFormat(cfDefault);
    //m_RichCtrl.SetWindowText(m_fileData.c_str());

    //for (vector<vector<pair<int, int>>>::iterator it = tagRanges.begin(); it != tagRanges.end(); ++it)
    /*{
        CString str;
        str.AppendFormat(_T("%d - "), it->size());
        for (vector<pair<int, int>>::iterator jt = it->begin(); jt != it->end(); ++jt)
        {
            str.AppendFormat(_T("(%d,%d) "), jt->first, jt->second);
        }
        m_ListBox.AddString(str.GetString());
    }*/

    UpdateData(FALSE);
    OnCancel();

}

std::vector<std::vector<std::pair<int, int>>> CInitDialog::GetTagRanges()
{
    return tagRanges;
}

std::string CInitDialog::GetFileData()
{
    return m_fileData;
}

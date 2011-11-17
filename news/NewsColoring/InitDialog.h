#pragma once


// CInitDialog dialog

class CInitDialog : public CDialog
{
	DECLARE_DYNAMIC(CInitDialog)

public:
	CInitDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInitDialog();

// Dialog Data
	enum { IDD = IDD_INITDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    CString m_fileName;
    std::string m_fileData;
    int m_minFreq;
    int m_minWordlen;
    int m_newsNum;
    std::vector<std::vector<std::pair<int, int>>> tagRanges;
    CWnd *m_pParentDialog;

public:
    afx_msg void OnBnClickedBtnrun();
    std::vector<std::vector<std::pair<int, int>>> GetTagRanges();
    std::string GetFileData();

};
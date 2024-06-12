
// CountLineCodingDlg.h : header file
//

#pragma once


// CCountLineCodingDlg dialog
class CCountLineCodingDlg : public CDialogEx
{
// Construction
public:
	CCountLineCodingDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COUNTLINECODING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMFCEditBrowseCtrl m_wndPath;
	CEdit m_wndCode;
	CEdit m_wndAddLines;
//	CEdit m_wndAddLines;
	CEdit m_wndModLines;
	CEdit m_wndDelLines;
	afx_msg void OnBnClickedCountButton();
	afx_msg void OnBnClickedResetButton();
	CProgressCtrl m_wndProgress;
	afx_msg void OnEnChangeModLineEdit();
};

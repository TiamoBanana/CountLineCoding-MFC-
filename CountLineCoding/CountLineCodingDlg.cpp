
// CountLineCodingDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CountLineCoding.h"
#include "CountLineCodingDlg.h"
#include "afxdialogex.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <afxpriv.h>
#include <locale>
#include <cctype>
namespace fs = std::filesystem;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCountLineCodingDlg dialog



CCountLineCodingDlg::CCountLineCodingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COUNTLINECODING_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCountLineCodingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE, m_wndPath);
	DDX_Control(pDX, IDC_EDIT, m_wndCode);
	//DDX_Control(pDX, IDC_ADD_LINE_EDIT, m_wndCode);
	DDX_Control(pDX, IDC_ADD_LINE_EDIT1, m_wndAddLines);
	DDX_Control(pDX, IDC_MOD_LINE_EDIT, m_wndModLines);
	DDX_Control(pDX, IDC_DEL_LINE_EDIT, m_wndDelLines);
	DDX_Control(pDX, IDC_PROGRESS, m_wndProgress);
}

BEGIN_MESSAGE_MAP(CCountLineCodingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COUNT_BUTTON, &CCountLineCodingDlg::OnBnClickedCountButton)
	ON_BN_CLICKED(IDC_RESET_BUTTON, &CCountLineCodingDlg::OnBnClickedResetButton)
	ON_EN_CHANGE(IDC_MOD_LINE_EDIT, &CCountLineCodingDlg::OnEnChangeModLineEdit)
END_MESSAGE_MAP()


// CCountLineCodingDlg message handlers

BOOL CCountLineCodingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	m_wndPath.EnableFolderBrowseButton();

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCountLineCodingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCountLineCodingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCountLineCodingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// Use the filesystem library based on the compiler
#if __has_include(<filesystem>)
	#include <filesystem>
	namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#else
	#error "Missing the <filesystem> header."
#endif


void CCountLineCodingDlg::OnBnClickedCountButton()
{
	// TODO: Add your control notification handler code here
	CString folderPath;
	m_wndPath.GetWindowText(folderPath);
	if (folderPath.IsEmpty())
	{
		AfxMessageBox(_T("Please select a folder path"));
		return;
	}

	// Convert CString to std::wstring
	std::wstring folderPathW(folderPath.GetString());

	// Get the code entered by the user
	CString userCode;
	m_wndCode.GetWindowText(userCode);
	if (userCode.IsEmpty())
	{
		AfxMessageBox(_T("Please enter a code"));
		return;
	}

	std::wstring userCodeW(userCode.GetString());
	std::wstring addStart = L"// add start " + userCodeW;
	std::wstring addEnd = L"// add end " + userCodeW;
	std::wstring modStart = L"// mod start " + userCodeW;
	std::wstring modEnd = L"// mod end " + userCodeW;
	std::wstring delStart = L"// del start " + userCodeW;
	std::wstring delEnd = L"// del end " + userCodeW;

	int totalAddCount = 0, totalModCount = 0, totalDelCount = 0;

	std::vector<std::wstring> files;

	try
	{
		for (const auto& entry : fs::recursive_directory_iterator(folderPathW))
		{
			if (entry.is_regular_file())
			{
				files.push_back(entry.path());
			}
		}
	}
	catch (const fs::filesystem_error& e)
	{
		AfxMessageBox(CString("Filesystem error: ") + CString(e.what()));
		return;
	}

	m_wndProgress.SetRange(0, static_cast<int>(files.size()));
	m_wndProgress.SetPos(0);

	for (size_t i = 0; i < files.size(); ++i)
	{
		const auto& filePath = files[i];
		std::ifstream file(filePath);
		if (!file.is_open())
		{
			CString msg = _T("Unable to open file: ");
			msg += filePath.c_str();
			AfxMessageBox(msg);
			continue;
		}

		std::string line;
		int addCount = 0, modCount = 0, delCount = 0;
		bool inAddBlock = false, inModBlock = false, inDelBlock = false;

		while (std::getline(file, line))
		{

			// Remove leading and trailing whitespace
			line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](char ch) { return !std::isspace(static_cast<unsigned char>(ch)); }));
			line.erase(std::find_if(line.rbegin(), line.rend(), [](char ch) { return !std::isspace(static_cast<unsigned char>(ch)); }).base(), line.end());

			// Skip blank lines
			if (line.empty()) continue;

			std::wstring wline(line.begin(), line.end());

			if (wline.find(addStart) != std::wstring::npos)
			{
				inAddBlock = true;
				continue;
			}
			if (wline.find(addEnd) != std::wstring::npos)
			{
				inAddBlock = false;
				continue;
			}
			if (wline.find(modStart) != std::wstring::npos)
			{
				inModBlock = true;
				continue;
			}
			if (wline.find(modEnd) != std::wstring::npos)
			{
				inModBlock = false;
				continue;
			}
			if (wline.find(delStart) != std::wstring::npos)
			{
				inDelBlock = true;
				continue;
			}
			if (wline.find(delEnd) != std::wstring::npos)
			{
				inDelBlock = false;
				continue;
			}

			if (inAddBlock) addCount++;
			if (inModBlock) modCount++;
			if (inDelBlock) delCount++;
		}

		totalAddCount += addCount;
		totalModCount += modCount;
		totalDelCount += delCount;

		m_wndProgress.SetPos(static_cast<int>(i + 1));
	}

	m_wndAddLines.SetWindowTextW(std::to_wstring(totalAddCount).c_str());
	m_wndModLines.SetWindowTextW(std::to_wstring(totalModCount).c_str());
	m_wndDelLines.SetWindowTextW(std::to_wstring(totalDelCount).c_str());
	/*CString folderPath;
	m_wndPath.GetWindowText(folderPath);
	if (folderPath.IsEmpty())
	{
		AfxMessageBox(_T("Please select a folder path"));
		return;
	}

	// Convert CString to std::wstring
	std::wstring folderPathW(folderPath.GetString());

	//Get the code entered by the user
	CString userCode;
	m_wndCode.GetWindowText(userCode);
	if (userCode.IsEmpty())
	{
		AfxMessageBox(_T("Please enter a code"));
		return;
	}

	std::wstring userCodeW(userCode.GetString());
	std::wstring addStart = L"// add start " + userCodeW;
	std::wstring addEnd = L"// add end " + userCodeW;
	std::wstring modStart = L"// mod start " + userCodeW;
	std::wstring modEnd = L"// mod end " + userCodeW;
	std::wstring delStart = L"// del start " + userCodeW;
	std::wstring delEnd = L"// del end " + userCodeW;


	//Init variable to count
	int totalAddCount = 0, totalModCount = 0, totalDelCount = 0;

	try
	{
		for (const auto& entry : fs::recursive_directory_iterator(folderPathW))
		{
			if (entry.is_regular_file())
			{
				std::ifstream file(entry.path());
				if (!file.is_open())
				{
					CString msg = _T("Unable to open file: ");
					msg += entry.path().c_str();
					AfxMessageBox(msg);
					continue;
				}

				std::string line;
				int addCount = 0, modCount = 0, delCount = 0;
				bool inAddBlock = false, inModBlock = false, inDelBlock = false;

				while (std::getline(file, line))
				{
					std::wstring wline(line.begin(), line.end());

					if (wline.find(addStart) != std::wstring::npos)
					{
						inAddBlock = true;
						continue;
					}
					if (wline.find(addEnd) != std::wstring::npos)
					{
						inAddBlock = false;
						continue;
					}
					if (wline.find(modStart) != std::wstring::npos)
					{
						inModBlock = true;
						continue;
					}
					if (wline.find(modEnd) != std::wstring::npos)
					{
						inModBlock = false;
						continue;
					}
					if (wline.find(delStart) != std::wstring::npos)
					{
						inDelBlock = true;
						continue;
					}
					if (wline.find(delEnd) != std::wstring::npos)
					{
						inDelBlock = false;
						continue;
					}

					if (inAddBlock) addCount++;
					if (inModBlock) modCount++;
					if (inDelBlock) delCount++;
				}

				totalAddCount += addCount;
				totalModCount += modCount;
				totalDelCount += delCount;
			}
		}
	}
	catch (const fs::filesystem_error& e)
	{
		AfxMessageBox(CString("Filesystem error: ") + CString(e.what()));
	}

	m_wndAddLines.SetWindowTextW(std::to_wstring(totalAddCount).c_str());
	m_wndModLines.SetWindowTextW(std::to_wstring(totalModCount).c_str());
	m_wndDelLines.SetWindowTextW(std::to_wstring(totalDelCount).c_str());*/
}


void CCountLineCodingDlg::OnBnClickedResetButton()
{
	// TODO: Add your control notification handler code here
	m_wndPath.SetWindowText(_T(""));
	m_wndCode.SetWindowText(_T(""));
	m_wndAddLines.SetWindowText(_T(""));
	m_wndModLines.SetWindowText(_T(""));
	m_wndDelLines.SetWindowText(_T(""));

	// Reset the progress b
	m_wndProgress.SetRange(0, 100);  
	m_wndProgress.SetPos(0);         
}


void CCountLineCodingDlg::OnEnChangeModLineEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

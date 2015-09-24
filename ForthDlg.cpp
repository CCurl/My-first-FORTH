
// ForthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Forth.h"
#include "ForthDlg.h"
#include "afxdialogex.h"

#include "vm.h"
#include "ForthOS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CForthDlg dialog

CForthDlg::CForthDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CForthDlg::IDD, pParent)
	, memSize(8192)
	, retStackSize(256)
	, commands(_T(".bs"))
	, output(_T(""))
	, stack(_T(""))
	, memDump(_T(""))
{
	myOS = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CForthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COMMANDS, commands);
	DDX_Text(pDX, IDC_MEMORY, memSize);
	DDX_Text(pDX, IDC_RETSTACK, retStackSize);
	DDX_Text(pDX, IDC_OUTPUT, output);
	DDX_Text(pDX, IDC_STACK, stack);
	DDX_Text(pDX, IDC_MEMDUMP, memDump);
}

BEGIN_MESSAGE_MAP(CForthDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CForthDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CForthDlg message handlers

BOOL CForthDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CForthDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CForthDlg::OnPaint()
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
HCURSOR CForthDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CForthDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (myOS == NULL)
	{
		myOS = new ForthOS(this->memSize);
		myOS->BootStrap();
	}
	myOS->ParseInput(commands);

	output = myOS->output;
	myOS->output.Empty();
	myOS->Dump(memDump);
	myOS->DumpStack(stack);
	UpdateData(FALSE);
}

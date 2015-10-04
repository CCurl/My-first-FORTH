
// ForthDlg.h : header file
//

#pragma once

#include "vm.h"
#include "ForthOS.h"
#include "afxwin.h"

// CForthDlg dialog
class CForthDlg : public CDialogEx
{
// Construction
public:
	CForthDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FORTH_DIALOG };

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
	afx_msg void OnBnClickedOk();

	ForthOS *myOS;
//	int memorySize;
//	int retStackSize;
	CString commands;
	long memSize;
	long retStackSize;
	CString output;
	CString stack;
	CString memDump;
	CEdit editMemDump;
};

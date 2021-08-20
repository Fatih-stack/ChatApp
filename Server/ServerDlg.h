
// ServerDlg.h : header file
//

#pragma once
#include "Connection.h"
#include <memory>

// CServerDlg dialog
class CServerDlg : public CDialog
{
private:
	std::thread m_Thread;
	HANDLE m_threadHandler;
	
// Construction
public:
	CServerDlg(CWnd* pParent = nullptr);	// standard constructor
	static UINT __cdecl pThreadHandler(LPVOID pParam);
	UINT pThreadFunc();
	void dispUpdate(CString str);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CEdit m_port;
	CListBox m_ServerList;
	std::shared_ptr<Connection> serverCon;
	afx_msg void OnEnChangeEdit2();
};


// ChatAppDlg.h : header file
//

#pragma once
#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include "Client.h"
#include <fstream>

// CChatAppDlg dialog
class CChatAppDlg : public CDialog
{
// Construction
public:
	CChatAppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
private:
	HANDLE m_Thread_handle;
	CWinThread* cTh;

protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void createMessage(CString sDisplay);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonLogout();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonFile();
	BOOL PreTranslateMessage(MSG* pMsg);
	CString m_ip;
	CString m_port;
	CString m_userName;
	CListBox m_listcntrl;
	CString m_msgcntrl;
	Client m_client;
	int nClientSocket;
	std::fstream file;
	CListBox m_Users;
	SOCKADDR_IN srv;
};


// ServerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CServerDlg dialog
CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_port);
	DDX_Control(pDX, IDC_LIST1, m_ServerList);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CServerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CServerDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT2, &CServerDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	serverCon = std::make_shared<Connection>();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//when start button pressed calls parent thread function
void CServerDlg::OnBnClickedButton1()
{
	m_Thread = std::thread(pThreadHandler, this);
}


void CServerDlg::OnBnClickedButton2()
{
	CDialog::OnCancel();
}

//thread handler make castings to call parent thread function
UINT __cdecl CServerDlg::pThreadHandler(LPVOID pParam)
{
	CServerDlg* pYourClass = reinterpret_cast<CServerDlg*>(pParam);
	pYourClass->pThreadFunc();
	return 0;
}

//add updates to server gui
void CServerDlg::dispUpdate(CString str)
{
	m_ServerList.AddString(str);
}

/***********************************************************
* take port number and make connections according to Connect
* function in Connection class
************************************************************/
UINT CServerDlg::pThreadFunc()
{
	CString portStr;
	
	GetDlgItem(IDC_EDIT2)->GetWindowText(portStr);
	int portNum = _ttoi(portStr);
	CString statusOne = (_T("Server initiated. Waiting for clients."));
	m_ServerList.AddString(statusOne);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	serverCon->Connect(portNum);
	return 0;


}

void CServerDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

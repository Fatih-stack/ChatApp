
// ChatAppDlg.cpp : implementation file
//
#define DEFAULT_BUFLEN 511
#include "pch.h"
#include "framework.h"
#include "ChatApp.h"
#include "ChatAppDlg.h"
#include "afxdialogex.h"
#include <string> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChatAppDlg::CChatAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CHATAPP_DIALOG, pParent)
	, m_userName(_T(""))
	, m_ip(_T(""))
	, m_port(_T(""))
	, m_msgcntrl(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listcntrl);
	DDX_Text(pDX, IDC_EDIT_NAME, m_userName);
	DDX_Text(pDX, IDC_EDIT_IP, m_ip);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Text(pDX, IDC_EDIT_MSG, m_msgcntrl);
	DDX_Control(pDX, IDC_LIST_Clients, m_Users);
}

BEGIN_MESSAGE_MAP(CChatAppDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CChatAppDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CChatAppDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CChatAppDlg::OnBnClickedButtonLogout)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatAppDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CChatAppDlg::OnBnClickedButtonFile)
END_MESSAGE_MAP()

// CChatAppDlg message handlers
BOOL CChatAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Add "About..." menu item to system menu.

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

void CChatAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CChatAppDlg::OnPaint()
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
HCURSOR CChatAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChatAppDlg::OnBnClickedButtonLogin()
{
	UpdateData(TRUE);
	int port = _ttoi((m_port));

	if (AfxSocketInit() == FALSE)
	{
		AfxMessageBox(_T("Socket initialization failed."));
		return;
	}
	
	if (m_client.Create() == FALSE)
	{
		AfxMessageBox(_T("Socket creation failed."));
		return;
	}
	
	if (m_client.Connect(m_ip, port) == FALSE)
	{
		AfxMessageBox(_T("Connection unsuccessful."));
		return;
	}
	m_client.pDlg = this;
	CString username;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(username);
	CString connInfo = username + _T(" is online now.");
	m_Users.AddString(username);
	AfxMessageBox(connInfo);
	int soStr = connInfo.GetLength();
	char* resultStr = new char[soStr];
	wsprintfA(resultStr, "%S", connInfo);
	m_client.Send(resultStr, soStr + 1);
	delete resultStr;
}

void CChatAppDlg::OnBnClickedButtonLogout()
{
	CString getUsername;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(getUsername);
	CString makeMsg = getUsername + _T(" is offline now.");
	int sof = makeMsg.GetLength();
	char* resultStr = new char[sof];
	wsprintfA(resultStr, "%S", makeMsg);
	makeMsg.ReleaseBuffer();
	m_client.Send(resultStr, sof + 1);
	AfxMessageBox(_T("Client Disconnected. "));
	m_client.Close();
}

void CChatAppDlg::OnBnClickedButtonSend()
{
	Client::isFile = false;
	CString userName;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(userName);
	CString createMsg;
	GetDlgItem(IDC_EDIT_MSG)->GetWindowText(createMsg);
	CString time = CTime::GetCurrentTime().Format("%H:%M");
	createMsg = userName + _T(" (@" + time + ") : ") + createMsg;

	int len = createMsg.GetLength();
	char* result = new char[len];
	wsprintfA(result, "%S", createMsg);
	createMsg.ReleaseBuffer();
	m_client.Send(result, len + 1);
	GetDlgItem(IDC_EDIT_MSG)->SetWindowText(_T(""));
}

BOOL CChatAppDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		int idCtrl = this->GetFocus()->GetDlgCtrlID();
		if (idCtrl == IDC_EDIT_MSG)
		{
			OnBnClickedButtonSend();
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CChatAppDlg::OnBnClickedOk()
{
	OnBnClickedButtonSend();
}

void CChatAppDlg::createMessage(CString display) {
	m_listcntrl.AddString(display);
}

void CChatAppDlg::OnBnClickedButtonFile()
{
	CFile FileObj;		//Variable for file operations
	CString sFilePath;
	const TCHAR szFilter[] = _T("TXT Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("txt"), NULL, OFN_HIDEREADONLY, szFilter, this);
	if (dlg.DoModal() == IDOK)
	{
		sFilePath = dlg.GetPathName();
	}
	else {
		return;
	}
	file.open((LPCTSTR)sFilePath, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		createMessage(_T("[ERROR] : File loading failed, Exititng."));
		return;
	}
	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	std::string s = std::to_string(contents.length());
	if (contents.length() > 15728640) {
		createMessage(_T("[ERROR] : File is too big, couldn't send"));
		return;
	}
	std::string str = "File size : " + s;
	int iResult = 0, len = 0, count = 0, sndbuflen = DEFAULT_BUFLEN;
	int size = contents.length();
	m_client.Send(str.c_str(), str.length());
	while (count < size) {
		char sndbuf[DEFAULT_BUFLEN+1];
		len = min(size - count, sndbuflen);
		memcpy(sndbuf, contents.data() + count, len);
		sndbuf[DEFAULT_BUFLEN] = '\0';
		// Send a buffer
		iResult = m_client.Send(sndbuf, len);
		if(len < 511)
			createMessage(_T("[LOG] : File Saved.\n"));
		file.close();
		Client::file.close();
		// iResult: Bytes sent
		if (iResult == SOCKET_ERROR) throw WSAGetLastError();
		else {
			if (iResult > 0) count += iResult;
			else break;
		}
	}
}
 


// ChatAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ChatApp.h"
#include "ChatAppDlg.h"
#include "afxdialogex.h"
#include <string> 
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define DEFAULT_BUFLEN 511

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
	m_client = std::make_shared<Client>();
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

/**********************************************************
* Make WSA initializations and start connection to server
* by taking port and ip from gui connects to server
* send "... is online now" msg to server receive msg from
* server and shows on GUI
*********************************************************/
void CChatAppDlg::OnBnClickedButtonLogin()
{
	UpdateData(TRUE);
	if (Client::initWSA() == false) {
		AfxMessageBox(_T("Error WSA init!!!"));
		return;
	};
	int port = _ttoi((m_port));
	std::string ip = CT2A(m_ip);
	m_client->setClientSocket(ip, port);
	m_client->Connect();
	//m_client.pDlg = this;
	CString username;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(username);
	std::string userName = CT2A(username);
	std::string connInfo = userName + " is online now.";
	m_Users.AddString(username);
	m_client->sendMSG(connInfo.c_str());
	m_client->receiveMSG();
	createMessage();
	m_client->receiveMSG();
	createMessage();
}

/****************************************************
* Close related client socket connection and shows
* which client is disconnetted as MessageBox
*****************************************************/
void CChatAppDlg::OnBnClickedButtonLogout()
{
	CString getUsername;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(getUsername);
	std::string userName = CT2A(getUsername);
	std::string makeMsg = userName + " is offline now.";
	AfxMessageBox(_T("Client Disconnected. "));
	m_client->~Client();
}

/****************************************************
* Send message which entered from user on GUI to server
* by taking username and message creates message format
* then send to server and receive message from server
* shows this message on GUI
*****************************************************/
void CChatAppDlg::OnBnClickedButtonSend()
{
	Client::isFile = false;
	CString userName;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(userName);
	CString createMsg;
	GetDlgItem(IDC_EDIT_MSG)->GetWindowText(createMsg);
	CString time = CTime::GetCurrentTime().Format("%H:%M");
	createMsg = userName + _T(" (@" + time + ") : ") + createMsg;

	std::string result = CT2A(createMsg);
	m_client->sendMSG(result.c_str());
	m_client->receiveMSG();
	createMessage();
	GetDlgItem(IDC_EDIT_MSG)->SetWindowText(_T(""));
}

/***************************************************
* Waits for key press event on GUI and if enter key
* is pressed in edit control sends message
****************************************************/
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
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SHIFT && 
			pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		int idCtrl = this->GetFocus()->GetDlgCtrlID();
		if (idCtrl == IDC_EDIT_MSG)
		{
			GetDlgItem(IDC_EDIT_MSG)->SetWindowText(_T("\r\n"));
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CChatAppDlg::OnBnClickedOk()
{
	OnBnClickedButtonSend();
}

//push messages to listbox
void CChatAppDlg::createMessage() {
	m_listcntrl.AddString(m_client->display);
}

/*********************************************************
* Take file path by opening filedialog and choosing file
* then Opens file take its contents and send piece piece
* to server
*********************************************************/
void CChatAppDlg::OnBnClickedButtonFile()
{
	CFile FileObj;		//Variable for file operations
	CString sFilePath;
	const TCHAR szFilter[] = _T("TXT Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("txt"), NULL, OFN_HIDEREADONLY, szFilter, this);
	if (dlg.DoModal() == IDOK)
	{
		sFilePath = dlg.GetPathName();		//take filepath with filename
	}
	else {
		return;
	}
	std::fstream file;
	file.open((LPCTSTR)sFilePath, std::ios::in | std::ios::binary);	//open file
	if (!file.is_open()) {
		m_client->display = _T("[ERROR] : File loading failed, Exititng.");
		createMessage();
		return;
	}
	//take file contents
	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	std::string s = std::to_string(contents.length());
	if (contents.length() > 15728640) {
		m_client->display = _T("[ERROR] : File is too big, couldn't send");
		createMessage();
		return;
	}
	std::string str = "File size : " + s;
	int iResult = 0, len = 0, count = 0, sndbuflen = DEFAULT_BUFLEN;
	int size = contents.length();
	//first send "File size : " message to specify file is coming to server
	m_client->sendMSG(str.c_str());
	m_client->receiveMSG();		//receive answer from server
	createMessage();			//shows answer on gui
	while (count < size) {		//send file piece piece to server until finish
		char sndbuf[DEFAULT_BUFLEN+1];	//create buff
		len = min(size - count, sndbuflen);
		memcpy(sndbuf, contents.data() + count, len);		//copy given length content to buffer
		sndbuf[DEFAULT_BUFLEN] = '\0';	//last char
		// Send a buffer
		iResult = m_client->sendMSG(sndbuf);	//send server
		m_client->receiveMSG();					//receive ans
		createMessage();						//show on listbox
		if(len < 511){
			m_client->display = _T("[LOG] : File Saved.\n");
			createMessage();
		}
		// iResult: Bytes sent
		if (iResult == SOCKET_ERROR) throw WSAGetLastError();
		else {
			if (iResult > 0) count += iResult;		//count send bytes
			else break;
		}
	}
}
 

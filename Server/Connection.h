#pragma once
class CServerDlg;
class Connection
{
public:
	Connection(CServerDlg* pDlg);
	~Connection(void);

	SOCKET m_serverSocket;
	SOCKET m_clientSocket;
	WSADATA wsa;
	CServerDlg* m_pDialog;
	CWinThread* m_cTh;
	HANDLE m_Thread_handle[20];
	void Connect(int nPort);
	static UINT __cdecl childThreadFunc(LPVOID pParam);
};


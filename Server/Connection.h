#pragma once
#include <thread>
#include <vector>

class CServerDlg;
class Connection
{
public:
	Connection();
	~Connection(void);

	SOCKET m_serverSocket;
	SOCKET m_clientSocket;
	WSADATA wsa;
	std::vector<std::thread> m_Thread_handle[20];
	void Connect(int nPort);
	static UINT __cdecl childThreadFunc(LPVOID pParam);
};


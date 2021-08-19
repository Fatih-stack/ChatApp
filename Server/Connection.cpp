#include "pch.h"
#include "Connection.h"
#include "Server.h"
#include "ServerDlg.h"
#include "unordered_map"

static SOCKET m_ClientSockets[100];
static int m_Clients;
Connection::Connection(CServerDlg* pDlg)
{
	m_pDialog = pDlg;
}

Connection::~Connection()
{
	closesocket(m_serverSocket);
	WSACleanup();
}

void Connection::Connect(int nPort)
{
	int res;
	SOCKADDR_IN srvInfo;

	res = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (res != 0)
	{
		AfxMessageBox(_T("Failed."));
		return;
	}

	if ((m_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		AfxMessageBox(_T("Could not create socket"));
	}

	srvInfo.sin_family = AF_INET;
	srvInfo.sin_addr.s_addr = INADDR_ANY;
	srvInfo.sin_port = htons(nPort);

	if (bind(m_serverSocket, (SOCKADDR*)&srvInfo, sizeof(srvInfo)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("Failed to bind."));
		exit(EXIT_FAILURE);
	}

	listen(m_serverSocket, 20);
	SOCKADDR_IN clntInfo;
	int inAddsize = sizeof(SOCKADDR_IN);
	m_Clients = 0;

	//Accept will break the listening loop and create a thread with client socket//

	while ((m_clientSocket = accept(m_serverSocket, (SOCKADDR*)&clntInfo, &inAddsize)) != INVALID_SOCKET)
	{
		SOCKADDR_STORAGE addr;
		char inIPad[INET6_ADDRSTRLEN];
		socklen_t len = sizeof(addr);
		getpeername(m_clientSocket, (SOCKADDR*)&addr, &len);
		SOCKADDR_IN* cCon = (SOCKADDR_IN*)&addr;
		inet_ntop(AF_INET, &cCon->sin_addr, inIPad, sizeof(inIPad));
		CString IPadv(inIPad);
		CString portVal;
		m_pDialog->GetDlgItem(IDC_EDIT2)->GetWindowText(portVal);
		CString t = CTime::GetCurrentTime().Format("%H:%M");
		CString stat = IPadv + " is connected on " + portVal + " [Time stamp: " + t + "]";
		m_pDialog->dispUpdate(stat);

		//Starting child thread whenever client connection is accepted//
		m_cTh = AfxBeginThread(childThreadFunc, (LPVOID)m_clientSocket);
		++m_Clients;
		m_ClientSockets[m_Clients] = m_clientSocket;

	}

	if (!m_clientSocket)
	{
		AfxMessageBox(_T("Failed to accept client."));
		return;
	}
}

UINT __cdecl Connection::childThreadFunc(LPVOID pParam)
{
	SOCKET pYourSocket = reinterpret_cast<SOCKET>(pParam);
	char* sendBuff;
	char recvBuff[550];
	sendBuff = "Initializing chat.";
	send(pYourSocket, sendBuff, strlen(sendBuff), 0);
	while ((recv(pYourSocket, recvBuff, 512, 0)) != SOCKET_ERROR)
	{
		int size = strlen(recvBuff) - 1;
		if (size > 512) size = 512;
		recvBuff[size] = '\0';
		//Broadcast to all clients//
		for (int brdcst = 1; brdcst <= m_Clients; brdcst++)
			send(m_ClientSockets[brdcst], recvBuff, strlen(recvBuff), 0);
	}
	return 0;
}
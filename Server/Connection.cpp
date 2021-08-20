#include "pch.h"
#include "Connection.h"
#include "Server.h"
#include "ServerDlg.h"
#include <memory>

static SOCKET m_ClientSockets[100];
static int m_Clients;
Connection::Connection()
{
}

//close socket and clean WSA
Connection::~Connection()
{
	closesocket(m_serverSocket);
	WSACleanup();
}

/**********************************************************************
* initializes WSA and create socket set ip and port numbers
* and bind then start to listen clients wait for clients to connect
* if any client is connected take its value make arrangements
* calls child thread function assign client socket num to thread array
**********************************************************************/
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

		//Starting child thread whenever client connection is accepted//
		++m_Clients;
		std::thread thread(childThreadFunc, (LPVOID)m_clientSocket);
		thread.join();
		m_ClientSockets[m_Clients] = m_clientSocket;
	}

	if (!m_clientSocket)
	{
		AfxMessageBox(_T("Failed to accept client."));
		return;
	}
}

/**************************************************************************
* take socket number by casting if first connection sends default message
* waits for incoming message after receive message send all clients
***************************************************************************/
UINT __cdecl Connection::childThreadFunc(LPVOID pParam)
{
	SOCKET pYourSocket = reinterpret_cast<SOCKET>(pParam);
	char* sendBuff;
	char recvBuff[550];
	sendBuff = "Initializing chat.";
	send(pYourSocket, sendBuff, strlen(sendBuff), 0);
	while ((recv(pYourSocket, recvBuff, 512, 0)) != SOCKET_ERROR)
	{
		int size = strlen(recvBuff);
		if (size > 512) size = 512;
		recvBuff[size] = '\0';
		//Broadcast to all clients//
		for (int brdcst = 1; brdcst <= m_Clients; brdcst++)
			send(m_ClientSockets[brdcst], recvBuff, strlen(recvBuff), 0);
	}
//	delete sendBuff;
	return 0;
}
#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Client.h"
#include "pch.h"
#include "ChatAppDlg.h"
#include <string>
#include <fstream>
#include <WinSock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool Client::isFile = false;
bool Client::m_isInitWsa = false;

Client::Client()
{
}

/*******************************************************
* Sets ip and port of server which client will connect * 
********************************************************/
void Client::setClientSocket(std::string ip, int port){
	if ((m_socketId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		AfxMessageBox(_T("Invalid Socket!!!"));
		return;
	}
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_port = htons(port);
	m_ServerAddr.sin_addr.s_addr = inet_addr(ip.c_str());
}

//close client socket
Client::~Client()
{
	closesocket(m_socketId);
}

//initialize WSA aif error occurs show on message box
bool Client::initWSA()
{
	if (!m_isInitWsa) {
		WSAData wSockData;
		WORD dllVersion = MAKEWORD(2, 2);           //version number winsock2.2 is initialized
		long success = WSAStartup(dllVersion, &wSockData);	//Start Winsock DLL
		if (success < 0)
		{
			AfxMessageBox(_T("WSA failed."));
			return false;
		}
		m_isInitWsa = true;
	}
	return true;
}

//Clean WSA
void Client::destroyWSA()
{
	if (m_isInitWsa)
	{
		WSACleanup();
		m_isInitWsa = false;
	}
}

//connect to server
void Client::Connect() const
{
	if (connect(m_socketId, (SOCKADDR*)&m_ServerAddr, sizeof(m_ServerAddr)) < 0) {
		AfxMessageBox(_T("Connection to server failed."));
		return;
	}
}

/*****************************************************************
* Receive message from server determine file or message if file
* create or open file and write message to file then close file
* if it is message take it and assign to display variable
******************************************************************/
bool Client::receiveMSG()
{
	char buffer[512] = {};
	if (Client::isFile) {
		std::fstream file;
		file.open("client_text.txt", std::fstream::in | std::fstream::out | std::fstream::app);
		if (!file.is_open()) {
			AfxMessageBox(_T("File creation failed."));
			return false;
		}	
		int valread = recv(m_socketId, buffer, 512, 0);
		buffer[valread] = '\0';
		file << buffer;
		file.close();
		return true;
	}
	int len;
	while ((len = recv(m_socketId, buffer, 512, 0)) > 0) {
		buffer[len] = '\0';
		CString rec_str(buffer);
		CString temp = rec_str.Mid(0, 12);
		if (temp == _T("File size : ")) {
			Client::isFile = true;
			return true;
		}
		display = rec_str;
		return true;
	}
	return false;
}

//sends message to server
int Client::sendMSG(const char *buffer) const
{
	return send(m_socketId, buffer, 512, 0);
}


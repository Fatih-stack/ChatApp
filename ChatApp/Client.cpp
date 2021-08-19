#include "Client.h"
#include "pch.h"
#include "ChatAppDlg.h"
#include <string>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool Client::isFile = false;

Client::Client()
{
}

Client::~Client()
{
}

std::fstream Client::file;
void Client::OnReceive(int nErrorCode)
{
	if (Client::isFile) {
		Client::file.open("client_text.txt", std::fstream::in | std::fstream::out | std::fstream::app);
		if (!Client::file.is_open()) {
			AfxMessageBox(_T("File creation failed."));
			return;
		}
		char buffer[512] = {};
		int valread = Receive(buffer, 512);
		Client::file << buffer;
		Client::file.close();
		return;
	}
	char recMsg[513];
	int len = Receive(recMsg, 512);
	recMsg[len] = '\0';
	CString rec_str = recMsg;
	CString temp = rec_str.Mid(0, 12);
	if (temp == _T("File size : ")) {
		isFile = true;
		return;
	}
	((CChatAppDlg*)pDlg)->createMessage(rec_str);
}

#pragma once
#include <afxsock.h>            // MFC socket extensions
#include <fstream>
class Client : public CSocket
{
public:
	Client();
	virtual ~Client();
	CDialog* pDlg;
	virtual void OnReceive(int nErrorCode);
	static bool isFile;
	static std::fstream file;
};

